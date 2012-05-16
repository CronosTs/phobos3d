/*
Phobos 3d
January 2011
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
#ifndef PH_GENERIC_FACTORY_H
#define PH_GENERIC_FACTORY_H

#include <boost/noncopyable.hpp>
#include <boost/intrusive/set.hpp>

#include <PH_Defs.h>
#include <PH_Exception.h>
#include <PH_String.h>

namespace Phobos
{

	template <typename T>
	class GenericFactory_c: boost::noncopyable
	{
	    public:
            template<typename A>
            struct ObjectCreatorComp_s
            {
                bool operator()(const String_c &name, const A &res) const
                {
                    return name.compare(res.GetName()) < 0;
                }

                bool operator()(const A &res, const String_c &name) const
                {
                    return res.GetName().compare(name) < 0;
                }
            };

			typedef boost::intrusive::set<T, boost::intrusive::constant_time_size<false> > ObjectCreatorSet_t;

		public:
			typedef typename T::ObjectType_t ObjectType_t;			

			void Register(T &creator)
			{
				setObjectCreators.insert(creator);
			}

			typename ObjectCreatorSet_t::const_iterator begin() const
			{
				return setObjectCreators.begin();
			}

			typename ObjectCreatorSet_t::const_iterator end() const
			{
				return setObjectCreators.end();
			}

		protected:
			GenericFactory_c()
			{
				//empty
			}

			const T &GetObjectCreator(const String_c &className) const
			{
				typename ObjectCreatorSet_t::const_iterator it = setObjectCreators.find(className, ObjectCreatorComp_s<T>());
				if(it == setObjectCreators.end())
					PH_RAISE(OBJECT_NOT_FOUND_EXCEPTION, "[EntityFactory_c::Create]", className);

				return *it;
			}

		protected:
            ObjectCreatorSet_t setObjectCreators;
	};


	typedef boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> > ObjectCreatorAutoUnlinkHook_t;

	template <typename T, typename Y>
	class ObjectCreatorBase_c: public ObjectCreatorAutoUnlinkHook_t
	{
		public:
			typedef T ObjectType_t;
			typedef Y ObjectCreatorProc_t;

		public:
			ObjectCreatorBase_c(const String_c &name, ObjectCreatorProc_t proc):
				strName(name),
				pfnCreateProc(proc)
			{
				if(proc == NULL)
				{
					std::stringstream stream;
					stream << "creator proc cant be null, entity " << name;
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[ObjectCreatorBase_c::ObjectCreatorBase_c]", stream.str());
				}
			}

			inline const String_c &GetName() const
			{
				return strName;
			}

			inline bool operator<(const ObjectCreatorBase_c &rhs) const
            {
                return strName.compare(rhs.strName) < 0;
            }

		private:
			String_c strName;

		protected:
			ObjectCreatorProc_t pfnCreateProc;
	};

	template <typename T, typename FACTORY>
	class ObjectCreator0_c: public ObjectCreatorBase_c<T, T(*)()>
	{
		public:
			typedef ObjectCreatorBase_c<T, T(*)()> BaseType_t;

		public:
			ObjectCreator0_c(const String_c &name, T(*proc)()):
				BaseType_t(name, proc)
			{				
				FACTORY::GetInstance().Register(*this);
			}
				
			T Create() const
			{
				return pfnCreateProc();
			}
	};

	template <typename T, typename PARAM1, typename FACTORY>
	class ObjectCreator1_c: public ObjectCreatorBase_c<T, T(*)(const PARAM1 &)>
	{
		public:
			typedef ObjectCreatorBase_c<T, T(*)(const PARAM1&)> BaseType_t;

		public:
			ObjectCreator1_c(const String_c &name, T(*proc)(const PARAM1 &)):
				BaseType_t(name, proc)
			{
				//GenericFactory_c<ObjectCreator_c<T> >::GetInstance().Register(*this);
				FACTORY::GetInstance().Register(*this);
			}
				
			T Create(const PARAM1 &name) const
			{
				return pfnCreateProc(name);
			}
	};

	template<typename T, typename PARAM1, typename PARAM2>
	class GenericFactory2_c;

	template <typename T, typename PARAM1, typename PARAM2, typename FACTORY>
	class ObjectCreator2_c: public ObjectCreatorBase_c<T, T(*)(const PARAM1 &, PARAM2 )>
	{
		public:
			typedef ObjectCreatorBase_c<T, T(*)(const PARAM1 &, PARAM2 )> BaseType_t;

			ObjectCreator2_c(const String_c &name,  T(*proc)(const PARAM1 &, PARAM2 ) ):
				BaseType_t(name, proc)
			{
				//GenericFactory2_c<ObjectCreator2_c, Y >::GetInstance().Register(*this);
				FACTORY::GetInstance().Register(*this);
			}

			T Create(const PARAM1 &name, PARAM2 param) const
			{
				return pfnCreateProc(name, param);
			}
	};

	template <typename T>
	class GenericFactory0_c: public GenericFactory_c<T>
	{
		public:
			typename GenericFactory_c<T>::ObjectType_t Create(const String_c &className) const
			{
				return this->GetObjectCreator(className).Create();
			}
	};

	template <typename T, typename PARAM1>
	class GenericFactory1_c: public GenericFactory_c<T>
	{
		public:
			typename GenericFactory_c<T>::ObjectType_t Create(const String_c &className, const PARAM1 &name) const
			{
				return this->GetObjectCreator(className).Create(name);
			}

	};

	template <typename T, typename PARAM1, typename PARAM2>
	class GenericFactory2_c: public GenericFactory_c<T>
	{
		public:
			typename GenericFactory_c<T>::ObjectType_t Create(const String_c &className, const PARAM1 &param1, PARAM2 param2) const
			{
				return this->GetObjectCreator(className).Create(param1, param2);
			}
	};

	template<typename T>
	struct ObjectCreatorComp_s
    {
        bool operator()(const String_c &name, const T &res) const
        {
            return name.compare(res.GetName()) < 0;
        }

        bool operator()(const T &res, const String_c &name) const
        {
            return res.GetName().compare(name) < 0;
        }
    };
}

#endif
