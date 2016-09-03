std::string tostring(long i);
inline const char* toCstring(long i) { return tostring(i).c_str(); }
/* Tries to determine boolean value of a string. Returns 1 for true, 0 for false
and -1 if unable to determine. */
int stringtobool(std::string boolstr);
/* These strcpy, strncpy, and strcat wrappers handle std:strings */
inline char* strcpy(char* dest, const std::string& src) { return strcpy(dest, src.c_str()); }
inline char* strncpy(char* dest, const std::string& src, size_t maxlen) { return strncpy(dest, src.c_str(), maxlen); }
inline char* strcat(char* dest, const std::string& src) { return strcat(dest, src.c_str()); }
/* These strcpy and strcat wrappers handle numbers */
inline char* strcpy(char* dest, long src) { return strcpy(dest, tostring(src)); }
inline char* strcat(char* dest, long src) { return strcat(dest, tostring(src)); }
/* This wrapper allows atoi to handle std::strings */
inline int atoi(const std::string& str) { return atoi(str.c_str()); }
/* This wrapper allows atof to handle std::strings */
inline double atof(const std::string& str) { return atof(str.c_str()); }
short creaturetype_string_to_enum(const std::string& ctname);
int augment_string_to_enum(const std::string& augmentname);
string attribute_enum_to_string(int attribute);
int attribute_string_to_enum(const std::string& attribute);
int skill_string_to_enum(std::string skillname);
int gender_string_to_enum(const std::string& gender);
int severtype_string_to_enum(const std::string& severtype);