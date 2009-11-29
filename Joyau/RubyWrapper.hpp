/*Copyright (C) 2009 Verhetsel Kilian

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/

#ifndef JOYAU_WRAPPER
#define JOYAU_WRAPPER

/** @defgroup Ruby
 *  Contains functions and macros in order to wrap C++ classes in Ruby.
 */

/** @addtogroup Ruby **/
/*@{*/

/** Returns Joyau's module **/
#define JOYAU_MOD rb_const_get(rb_cObject, rb_intern("Joyau"))

/** Cast, which converts a prototype so that it can be used
 *  with ruby's function.
 */
#define RPROTO(proto) (VALUE(*)(...))&proto // A boring cast...

// Easier to understand

/** Defines a ruby function
 *  @param func function's name
 *  @param proto function's implementation (automatically casted)
 *  @param argc argument count
 */
#define defFunc(func, proto, argc) \
    rb_define_global_function(func, RPROTO(proto), argc)

/** Defines a ruby method
 *  @param klass class in which the method should be defined.
 *  @param func method's name
 *  @param proto method's implementation (automatically casted)
 *  @param argc argument count
 */
#define defMethod(klass, func, proto, argc) \
    rb_define_method(klass, func, RPROTO(proto), argc)

/** Defines a ruby class method
 *  @param klass class in which the method should be defined.
 *  @param func method's name
 *  @param proto method's implementation (automatically casted)
 *  @param argc argument count
 */
#define defClassMethod(klass, func, proto, argc) \
    rb_define_singleton_method(klass, func, RPROTO(proto), argc)

/** Defines a ruby alias
 *  @param klass class in which the alias should be defined
 *  @param oldName symbol's old name
 *  @param newName symbol's new name
 */
#define defAlias(klass, oldName, newName) \
    rb_define_alias(klass, newName, oldName)

/** Defines a function in a module.
 *  @param mod module in whichthe function should be added.
 *  @param func function's name
 *  @param proto function's implementation (automatically casted)
 *  @param argc argument count
 */
#define defModFunc(mod, func, proto, argc) \
    rb_define_module_function(mod, func, RPROTO(proto), argc)

/** Like defFunc, but works for template functions which takes no arguments. */
#define defTplMethod(klass, func, proto) \
    { VALUE(*tmp)(VALUE) = &proto; \
	  rb_define_method(klass, func, (VALUE(*)(...))tmp, 0); }

/** Like defFunc, but works for template functions which takes 1 argument. **/
#define defTplMethod1(klass, func, proto) \
    { VALUE(*tmp)(VALUE, VALUE) = &proto; \
	  rb_define_method(klass, func, (VALUE(*)(...))tmp, 1); }

/** Like defFunc, but works for template functions which takes 2 arguments. **/
#define defTplMethod2(klass, func, proto) \
    { VALUE(*tmp)(VALUE, VALUE, VALUE) = &proto; \
	  rb_define_method(klass, func, (VALUE(*)(...))tmp, 2); }

/** Defines a module **/
#define defModule(name) rb_define_module_under(JOYAU_MOD, name)

/** Defines a constant **/
#define defConst rb_define_const


#ifdef JOYAU_RB_19
# define JOYAU_RB_VERSION "1.9"
#else
# define JOYAU_RB_VERSION "1.8"
# define JOYAU_RB_18
#endif

/** Destroys the object info. Used when a ruby object is destroyed. **/
template<typename T> void wrapped_free(void *info)
{
   delete (T*)info;
}

/** Returns a ruby object, whose class is info, and in which we've
 *  wrapped a new object, whose type is T. 
 */
template<typename T> VALUE wrap(int argc, VALUE *argv, VALUE info)
{
   T *ptr = new T;
   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<T>, ptr);
   return tdata;
}

/** Returns a pointer wrapped in val **/
template<typename T> T *getPtr(VALUE val)
{
   T *ptr;
   Data_Get_Struct(val, T, ptr);
   return ptr;
}

/** Returns a reference to the object wrapped in val. **/
template<typename T> T &getRef(VALUE val)
{
   return (*getPtr<T>(val));
}

/** Don't free the object. Used in some cases, when we need the C++ object
 *  even if the ruby object has been destroyed.
 */
inline void no_free(void *info) {}

/** Crates a Ruby object from a C++ object.
 *  @param info ruby class
 *  @param val ruby object.
 *  @param exist when true, the object won't be copied.
 */
template<typename T> VALUE createObject(VALUE info, T &val, bool exist = false)
{
   if (exist)
      return Data_Wrap_Struct(info, 0, no_free, &val);

   // If the object won't exist anymore, we have to copy it.
   T *ptr = new T(val);
   VALUE tdata = Data_Wrap_Struct(info, 0, wrapped_free<T>, ptr);
   
   return tdata;
}

/** Defines a Ruby class.
 *  @param name class name.
 *  @param father father class.
 */
template<typename T> VALUE defClass(const char *name, 
				    VALUE father = rb_cObject)
{
   VALUE ret = rb_define_class_under(JOYAU_MOD, name, father);
   VALUE (*func)(int, VALUE*, VALUE) = &wrap<T>;
   
   rb_define_singleton_method(ret, "new", (VALUE(*)(...))func, -1);
   return ret;
}

/** Returns a Ruby class.
 *  @param name class name.
 */
inline VALUE getClass(const char *name)
{
   return rb_const_get(JOYAU_MOD, rb_intern(name));
}

/** Returns a Ruby function.
 *  @param name function name.
 */
inline VALUE getFunc(const char *name)
{
   return rb_intern(name);
}

/** Defines a Ruby class.
 *  @param name class name.
 *  @param father father class name.
 */
template<typename T> VALUE defClass(const char *name, const char *father)
{
   return defClass<T>(name, getClass(father));
}

/** 
 * @class RubyObject
 * Allows to create ruby objects from C++ object.
 */
class RubyObject
{
public:
   /** Sets the Ruby class.
    *  @param val class name (has to exist).
    */
   void setClass(const std::string &val) { klass = getClass(val.c_str()); }

   /** Returns a ruby object, without copying this. **/
   virtual VALUE toRuby()
   {
      return createObject(klass, *this, true);
   }
private:
   VALUE klass;
};

/**
 *  @class RubyReject
 *  A class which yields an object in Ruby, and then check
 *  if the evaluated expression is true.
 */
template<typename T>
struct RubyReject
{
   /** Yields the object, and returns true if the evaluated expression is. **/
   bool operator()(T &obj)
   {
      VALUE rbObj = obj.toRuby();
      return rb_yield(rbObj) == Qtrue;
   }
};

/**
 *  @class RubyException
 *  Class which allows to raise Ruby exception from a C++ one.
 */
class RubyException: public std::exception {
public:
   /** 
    *  Creates a new exception, whose class is rbKlass, and whose message is
    *  str.
    */
   RubyException(VALUE rbKlass, const std::string &str) throw():
      _rbKlass(rbKlass), _str(str) {}
   virtual ~RubyException() throw() {}

   virtual const char* what() const throw() {
      return _str.c_str();
   }

   /* Raise the exception in Ruby */
   void rbRaise() const throw() { rb_raise(_rbKlass, _str.c_str()); }
private:
   VALUE _rbKlass;
   std::string _str;
};

inline VALUE safe_load(VALUE arg) {
   return rb_require(StringValuePtr((arg = rb_gv_get("$file_to_load"), arg)));
}

/** Run the script called filename **/
inline VALUE runScript(const std::string &filename) {
   int error;
   rb_gv_set("$file_to_load", rb_str_new2(filename.c_str()));
   VALUE res = rb_protect(safe_load, 0, &error);
   if (error != 0)
      throw RubyException(0, "");
   return res;
}

/** Converts a ruby hash to an OSL_COLOR. **/
inline OSL_COLOR hash2col(VALUE hash)
{
   if (TYPE(hash) == T_HASH) {
      int r = FIX2INT(rb_hash_aref(hash, rb_str_new2("r")));
      int g = FIX2INT(rb_hash_aref(hash, rb_str_new2("g")));
      int b = FIX2INT(rb_hash_aref(hash, rb_str_new2("b")));
      int a = FIX2INT(rb_hash_aref(hash, rb_str_new2("a")));

      return RGBA(r, g, b, a);
   }
   else if (TYPE(hash) == T_FIXNUM) {
      return FIX2INT(hash);
   }
   else
      return hash2col(rb_funcall(hash, getFunc("to_hash"), 0));
}

/** Converts an OSL_COLOR to a ruby hash. **/
inline VALUE col2hash(OSL_COLOR col)
{
   u8 red, green, blue, alpha;
   oslRgbaGet8888(col, red, green, blue, alpha);
   
   VALUE hash = rb_hash_new();
   rb_hash_aset(hash, rb_str_new2("r"), INT2FIX(red));
   rb_hash_aset(hash, rb_str_new2("g"), INT2FIX(green));
   rb_hash_aset(hash, rb_str_new2("b"), INT2FIX(blue));
   rb_hash_aset(hash, rb_str_new2("a"), INT2FIX(alpha));

   return hash;
}

/*@}*/

#endif
