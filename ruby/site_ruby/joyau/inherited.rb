#
# inherited.rb - Fixes for class which inherits Joyau's classes.
#

# :stopdoc:

#
# Copyright 2009-2010 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:

module Joyau
  # This module is still here for compatibility. The bug it was trying to fix
  # has really been fixed.
  module Inherited
    def new(*args) # :nodoc:
      ret = allocate
      ret.send(:initialize, *args)
      return ret
    end
  end
end

class Module
  def joyau_inherited
    class << self
      include Joyau::Inherited
    end
  end
end
