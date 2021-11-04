#ifndef src_lj_defined_h_
#define src_lj_defined_h_

#define disable_copy(class_name)                            \
   class_name(const class_name& ) = delete;                 \
   class_name operator= (const class_name& ) = delete;      

#endif // ! src_lj_defined_h_