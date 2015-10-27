/**
 * \file
 * \brief dummyDeleter() declaration
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-10-27
 */

#ifndef INCLUDE_DISTORTOS_MEMORY_DUMMYDELETER_HPP_
#define INCLUDE_DISTORTOS_MEMORY_DUMMYDELETER_HPP_

#include <type_traits>

namespace distortos
{

namespace memory
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions' declarations
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief A "no-op" dummy deleter that can be used with std::unique_ptr and automatic storage that is trivially
 * destructible.
 *
 * \param T is the real type of storage, must be trivially destructible
 * \param U is the type of \a storage pointer
 *
 * \param [in] storage is a pointer to storage
 */

template<typename T, typename U>
void dummyDeleter(U*)
{
	static_assert(std::is_trivially_destructible<T>::value == true,
			"memory::dummyDeleter() cannot be used with types that are not trivially destructible!");
}

}	// namespace memory

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_MEMORY_DUMMYDELETER_HPP_
