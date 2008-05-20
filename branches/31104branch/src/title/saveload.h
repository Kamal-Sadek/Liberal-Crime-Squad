#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/map.hpp>

#define save() saveloadBoost<std::ofstream,boost::archive::text_oarchive>("save.txt")
#define autosave() saveloadBoost<std::ofstream,boost::archive::text_oarchive>("autosave.txt")
#define load() saveloadBoost<std::ifstream,boost::archive::text_iarchive>("save.txt")

#include "saveload.cpp"

#endif //SAVELOAD_H_INCLUDED