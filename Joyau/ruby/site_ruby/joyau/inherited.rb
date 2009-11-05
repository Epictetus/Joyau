#
# inherited.rb - Fixes for class which inherits Joyau's classes.
#

# :stopdoc:

#
# Copyright 2009 Verhetsel Kilian
# This program can be distributed under the terms of the GNU GPL.
# See the file COPYING.
#

# :startdoc:

module Joyau
  # There can be problem with classes which inherits Joyau's classes.
  # This module fixes their new method, in order to call initialize.
  module Inherited
    def new(*args) # :nodoc:
      ret = super()
      ret.send(:initialize, *args)
      return ret
    end
  end
end

class Module
  # Fix for clases which inherits Joyau's classes.
  def joyau_inherited
    class << self
      include Joyau::Inherited
    end
  end
end
