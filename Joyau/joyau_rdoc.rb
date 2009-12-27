require 'rdoc'
require 'rdoc/rdoc'
require 'rdoc/parser/c'

class RDoc::Parser::Joyau < RDoc::Parser::C
  parse_files_matching /\.[ch]pp$/
  
  def initialize(top_level, file_name, content, options, stats)
    super

    @known_classes.merge!("joyau" => "Joyau")
  end

  def do_aliases
    super

    @content.scan(/defAlias\((\w+), \"([\[\]\?!\=\w]+)\", \"([\[\]\?!\=\w]+)\"\)/) do
      |var_name, old_name, new_name|
      
      class_name = @known_classes[var_name] || var_name
      class_obj  = find_class(var_name, class_name)

      as = class_obj.add_alias RDoc::Alias.new("", old_name, new_name, "")

      @stats.add_alias as
    end
  end

  def do_classes
    super

    @content.scan(/(\w+) = defModule\(\"(\w+)\"\)/) do |var_name, class_name|
      in_module = "joyau"
      handle_class_module(var_name, "module", class_name, nil, in_module)
    end

    @content.scan(/(\w+) = defClass<([:\w]+)>\(\"(\w+)\"\)/) do 
      |var_name, cpp_class, class_name|
      next if class_name.include? '"'

      in_module = "joyau"
      parent = "rb_cObject"

      handle_class_module(var_name, "class", class_name, parent, in_module)
      handle_method("singleton_method", var_name, "new",
                    "wrap<#{cpp_class}>", -1) if 
        @content.include? "wrap<#{cpp_class}>"
    end

    @content.scan(/(\w+) = defClass<([:\w]+)>\(\"(\w+)\", \"(\w+)\"\)/) do
      |var_name, cpp_class, class_name, parent|
      
      in_module = "joyau"
      parent = "Joyau::#{parent}"
     
      handle_class_module(var_name, "class", class_name, parent, in_module)
      handle_method("singleton_method", var_name, "new",
                    "wrap<#{cpp_class}>", -1) if 
        @content.include? "wrap<#{cpp_class}>"
    end

    # Template classes are hardcoded here.
    @content.scan(/defineMsgSelecterKlass<(\w+)>\(\"(\w+)\"\)/) do
      |var_name, class_name|
      
      in_module = "joyau"
      parent = "Joyau::Drawable"

      handle_class_module(var_name, "class", class_name, parent, in_module)

      handle_method("singleton_method", var_name, "new",
                    "wrap<#{var_name}>", -1)

      handle_selecter(var_name)

      handle_method("method", var_name, "conf", "MsgSelecter_conf", 0, 
                   "MsgSelecterImpl.hpp")
      handle_method("method", var_name, "focusConf", "MsgSelecter_focusConf", 
                    0, "MsgSelecterImpl.hpp")

      handle_method("method", var_name, "conf=", "MsgSelecter_setConf", 
                    1, "MsgSelecterImpl.hpp")
      handle_method("method", var_name, "focusConf=", 
                    "MsgSelecter_setFocusConf", 1, "MsgSelecterImpl.hpp")
    end

    @content.scan(/defineGenericSelecter<(\w+)>\(\"(\w+)\"\)/) do
      |var_name, class_name|
      in_module = "joyau"
      parent = "Joyau::Drawable"

      handle_class_module(var_name, "class", class_name, parent, in_module)

      handle_method("singleton_method", var_name, "new",
                    "wrap<#{var_name}>", -1)

      handle_selecter(var_name)
    end
  end

  def handle_selecter(var_name)
    handle_method("method", var_name, "select", "GenericSelecter_select", 1, 
                  "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "<<", "GenericSelecter_addItem", 1, 
                  "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "resize", "GenericSelecter_resize", 2, 
                  "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "selected", "GenericSelecter_selected", 
                  0, "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "item", "GenericSelecter_select", 1, 
                  "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "index", "GenericSelecter_index", 0, 
                  "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "size", "GenericSelecter_size", 0, 
                  "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "focus=", "GenericSelecter_setFocus", 
                  0, "GenericSelecterImpl.hpp")
    handle_method("method", var_name, "focus", "GenericSelecter_focus", 0, 
                  "GenericSelecterImpl.hpp")
  end

  def do_constants
    super

    @content.scan(/defConst\((\w+), \"(\w+)\", (.+)\)/) do 
      |var_name, const_name, definition|
      
      type = "const"
      var_name = "rb_cObject" if var_name == "rb_mKernel"

      handle_constants(type, var_name, const_name, definition)
    end
  end

  def do_methods
    super
    
    @content.scan(/defFunc\(\"([\[\]\?!\=\w]+)\", (\w+), ([\-0-9]+)\)/) do
      |meth_name, meth_body, param_count|
      handle_method("method", "rb_mKernel", meth_name,
                    meth_body, param_count)
    end

    @content.scan(/defMethod\((\w+), \"([\[\]\?!\=\w]+)\", (\w+), ([\-0-9]+)\)/) do
      |var_name, meth_name, meth_body, param_count|
      handle_method("method", var_name, meth_name,
                    meth_body, param_count)
    end

    @content.scan(/defModFunc\((\w+), \"([\[\]\?!\=\w]+)\", (\w+), ([\-0-9]+)\)/) do 
      |var_name, meth_name, meth_body, param_count|
      
      handle_method("module_function", var_name, meth_name,
                    meth_body, param_count)
    end

    @content.scan(/defClassMethod\((\w+), \"([\[\]\?!\=\w]+)\", (\w+), ([\-0-9]+)\)/) do 
      |var_name, meth_name, meth_body, param_count|
      
      handle_method("singleton_method", var_name, meth_name,
                    meth_body, param_count)
    end
  end 
end
