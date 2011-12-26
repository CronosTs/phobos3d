/*
Phobos 3d
September 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#include "PH_MapLoader.h"

#include <iostream>
#include <vector>

#include <rapidxml.hpp>

#include <PH_Dictionary.h>
#include <PH_DictionaryHive.h>
#include <PH_DictionaryManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

#define CUSTOM_PROPERTY_NODE_NAME "CUSTOMPROPERTIES"
#define PROPERTY_NODE_NAME "PROPERTY"

namespace Phobos
{
	void MapLoader_c::OnBoot()
	{
		DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

		ipCurrentLevelHive = manager->CreateCustomHive("LevelInfo");
		ipStaticEntitiesHive = manager->CreateCustomHive("StaticEntities");
		ipDynamicEntitiesHive = manager->CreateCustomHive("DynamicEntities");		
	}

	static const char *GetChildNodeValue(const rapidxml::xml_node<> &element, const char *nodeName)
	{		
		if(rapidxml::xml_node<> *child = element.first_node(nodeName))
		{
			if(rapidxml::xml_attribute<> *valueAtr =child->first_attribute("value"))
			{
				return valueAtr->value();
			}
		}

		return NULL;
	}

	static bool IsEditorOnly(const rapidxml::xml_node<> &element)
	{
		if(rapidxml::xml_node<> *custom = element.first_node(CUSTOM_PROPERTY_NODE_NAME))
		{
			for(const rapidxml::xml_node<> *elem = custom->first_node(PROPERTY_NODE_NAME); elem; elem = elem->next_sibling(PROPERTY_NODE_NAME))
			{
				const rapidxml::xml_attribute<> *name = elem->first_attribute("id");
				if(strcmp(name->value(), "editorOnly") == 0)
				{
					const rapidxml::xml_attribute<> *value = elem->first_attribute("value");
					if(strcmp(value->value(), "true")==0)
						return true;
				}
			}
		}

		return false;
	}

	static bool ContainsCustomProperties(const rapidxml::xml_node<> &element)
	{
		if(rapidxml::xml_node<> *custom = element.first_node(CUSTOM_PROPERTY_NODE_NAME))
			return custom->first_node(PROPERTY_NODE_NAME) != NULL;
		else
			return false;
	}

	static bool IsDynamicEntity(const rapidxml::xml_node<> &element)
	{
		if(ContainsCustomProperties(element))
			return true;

		const rapidxml::xml_attribute<> *attribute = element.first_attribute("typename");
		return (strcmp(attribute->value(), "Marker Object") == 0) || (strcmp(attribute->value(), "Camera Object") == 0);
	}

	static void LoadProperties(DictionaryPtr_t dict, const rapidxml::xml_node<> &element)
	{
		Log_c::Stream_c stream = Kernel_c::GetInstance().LogStream();

		for(const rapidxml::xml_node<> *elem = element.first_node(PROPERTY_NODE_NAME); elem; elem = elem->next_sibling(PROPERTY_NODE_NAME))
		{
			const rapidxml::xml_attribute<> *name = elem->first_attribute("id");
			const rapidxml::xml_attribute<> *value = elem->first_attribute("value");

			if(name == NULL)
			{
				stream << "[MapLoader_c::LoadDictionary] id attribute not found on CUSTOMPROPERTY node\n";
				continue;
			}

			if(value == NULL)
			{
				stream << "[MapLoader_c::LoadDictionary] value attribute not found on CUSTOMPROPERTY node\n";
				continue;
			}

			dict->AddValue(name->value(), value->value());
		}
	}

	static void LoadDictionary(DictionaryPtr_t dict, const rapidxml::xml_node<> &element)
	{
		for(const rapidxml::xml_attribute<> *atr = element.first_attribute(); atr; atr = atr->next_attribute())
		{
			dict->AddValue(atr->name(), atr->value());
		}		

		const rapidxml::xml_node<> *custom = element.first_node(CUSTOM_PROPERTY_NODE_NAME);

		LoadProperties(dict, element);

		if(custom)
			LoadProperties(dict, *custom);
	}

	void MapLoader_c::LoadOgitor(const String_c &fileName)
	{		
		rapidxml::xml_document<> doc;

		std::ifstream input(fileName.c_str(), std::ios_base::in);

		if(input.fail())			
			PH_RAISE(FILE_NOT_FOUND_EXCEPTION, "MapLoader_c::LoadOgitor", "'" + fileName + "' not found");

		std::vector<char> fileData( (std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
		fileData.push_back('\0');
		
		doc.parse<0>(&fileData[0] );	

		ipCurrentLevelHive->RemoveAllChildren();
		ipStaticEntitiesHive->RemoveAllChildren();
		ipDynamicEntitiesHive->RemoveAllChildren();		

		rapidxml::xml_node<> *root = doc.first_node();
		if(root == NULL)
			PH_RAISE(PARSER_EXCEPTION, "MapLoader_c::LoadOgitor", "'" + fileName + "' appears to be empty");		

		for(rapidxml::xml_node<> *elem = root->first_node("OBJECT");elem; elem = elem->next_sibling())
		{			
			try
			{
				rapidxml::xml_attribute<> *nameAttribute = elem->first_attribute("name");
				if(nameAttribute == NULL)
				{
					Kernel_c::GetInstance().LogMessage("[MapLoader_c::LoadOgitor] Object without name, ignored");
					continue;
				}

				bool dynamicEntity = IsDynamicEntity(*elem);
				if(dynamicEntity && IsEditorOnly(*elem))
					continue;
				
				DictionaryPtr_t dict = Dictionary_c::Create(nameAttribute->value());

				LoadDictionary(dict, *elem);

				(dynamicEntity ? ipDynamicEntitiesHive : ipStaticEntitiesHive)->AddDictionary(dict);				
			}
			catch(Exception_c &e)
			{
				Kernel_c::GetInstance().LogMessage(e.what());
			}			
		}

		DictionaryPtr_t dict = Dictionary_c::Create("Project");

		//load project data
		if(rapidxml::xml_node<> *project = root->first_node("PROJECT"))
		{
			if(const char *caelumDir = GetChildNodeValue(*project, "CAELUMDIR"))
				dict->AddValue("CaelumDir", caelumDir);			

			if(const char *terrainDir = GetChildNodeValue(*project, "TERRAINDIR"))
				dict->AddValue("TerrainDir", terrainDir);
		}
		ipCurrentLevelHive->AddDictionary(dict);

		//fill out basic level data
		dict = Dictionary_c::Create("LevelFile");

		dict->AddValue("PathName", fileName);

		Path_c path(fileName);
		Path_c filePath, onlyFileName;
		path.ExtractPathAndFilename(&filePath, &onlyFileName);

		dict->AddValue("Path", filePath.GetStr());
		dict->AddValue("FileName", onlyFileName.GetStr());

		ipCurrentLevelHive->AddDictionary(dict);
		
	}
}
