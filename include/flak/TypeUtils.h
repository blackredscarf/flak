/*

Copyright 2019 flak authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#ifndef FLAK_TYPE_UTILS_H
#define FLAK_TYPE_UTILS_H

#include <iterator>

namespace flak {

template <typename Container>
struct ContainerTraits {
    typedef typename Container::value_type value_type;
};

//template <>
//struct ContainerTraits <std::string> {
//    typedef char value_type;
//};

template <typename Container>
struct ContainerTraits <Container*> {
    typedef typename std::iterator_traits<Container*>::value_type value_type;
};

}

#endif //FLAK_TYPE_UTILS_H
