#
# buffer_library.rb - Creates a library of graphical ressources.
#

# :stopdoc:

#
# Copyright 2009-2010 Verhetsel Kilian
# This program can be distributed under the terms of the GNU LGPL.
# See the file COPYING.
#

# :startdoc:

module Joyau
  # Error raised by BufferLibrary when you try to register an invalid
  # loader.
  class InvalidLoader < StandardError; end
  
  # Error raised by a Buffer library when the ressource name is not correct.
  class InvalidRessourceName < StandardError; end

  # The BufferLibrary allows to register your own loader for graphical ressources
  # (a.k.a Buffer). This allows to create a library of red buffers easily, whose
  # sprites could be loaded simply with:
  #   Joyau::Sprite.from_res_name("red:my_picture.png")
  # The matching regex being simply /^red:(.+)/.
  #
  # Another example :
  #   module MyLoader
  #     include Joyau::BufferLibrary
  #
  #     @buffers = {}
  #     @regex = /my_loader:(.+)/
  #
  #     def self.[](res_name)
  #       @buffers[res_name] || create_buffer(res_name)
  #     end
  #
  #     def self.create_buffer(res_name)
  #       filename = nil
  #       if res_name =~ regex
  #         filename = $1
  #       else
  #         raise Joyau::InvalidRessourceName, ...
  #       end
  #       @buffers[res_name] = Joyau::Buffer[filename]
  #       # modify the buffer.
  #       return @buffers[res_name]
  #     end
  #     
  #     register_loader @regex
  #   end
  #
  # And now you can simply use your loader :
  #   Joyau::Sprite.from_res_name("my_loader:picture.png")
  module BufferLibrary
    @@regexps = {}

    class << self
      # Register an image loader, which must respond to [].
      # Each ressource name matching with the regex will be given to that method,
      # which should return a buffer.
      def register_loader(mod, regex)
        if mod.respond_to? :[]
          @@regexps[regex] = mod
        else
          raise InvalidLoader, "#{mod} is not a valid image Loader."
        end
      end

      # Returns a buffer matching the ressource name. It uses either
      # the loader whose regex match with the ressource name, or the
      # built-in loader.
      def [](res_name)
        @@regexps.each do |regex, loader|
          if res_name =~ regex
            return loader[res_name]
          end
        end

        # If the string didn't match any of the previous regex,
        # we use the built-in loader.
        return Joyau::Buffer[res_name, false]
      end

      def included(by) # :nodoc:
        class << by
          def register_loader(regex)
            Joyau::BufferLibrary.register_loader(self, regex)
          end
        end
      end
    end
  end

  class Sprite
    class << self
      # Creates a sprite from its ressource name. See BufferLibrary for
      # more information.
      def from_res_name(res_name)
        obj = new(BufferLibrary[res_name])
        obj.res_name = res_name

        return obj
      end
    end
  end
end
