#ifndef KEY_SCHEDULE_HPP_
#define KEY_SCHEDULE_HPP_

#include "Types.hpp"
#include "Tables.hpp"


namespace aes{

RoundKeys expandKey(const Key& Key);

}  // Namespace AES


#endif