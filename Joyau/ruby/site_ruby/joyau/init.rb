
class UndefinedModule < StandardError; end

module Joyau
  #
  # == Usage ==
  #
  # This module allows to inits some services, and to stop them easily :
  #
  #  require 'joyau/init'
  #  Joyau.init(:intrafont, :audio)
  #  # Do something
  #  Joyau.stop
  #
  # This is a simple use, but notice that dependencies are managed: Joyau.initLib
  # as well as Joyau.initGfx are run, because Intrafont depends on them.
  #
  # The default modules are:
  #  1. :lib, the main library.
  #  2. :gfx, the graphic module. Depends on :lib.
  #  3. :wlan, the net-related modules.
  #  4. :intrafont, the Intrafont module. Depends on :gfx
  #  5. :audio, the audio module.
  #  6. :usb, the USB module.
  #
  # Every module you load through Joyau.init is stopped when Joyau.stop. You
  # can stop module during your program's execution as well:
  #
  #  Joyau.stop(:something, :another_mod) # Stops :something and :another_mod
  #  Joyau.stop # Stops everything
  #
  # == Module creation ==
  #
  # If you created a module, you can make it loadable through this interface:
  #  class MyModule
  #    def self.init; puts "My module has been inited !"
  #    def self.stop; puts "My module has been stopped !"
  #  end
  #  Joyau::Init.register(:my_module, MyModule.method(:init), 
  #                                   MyModule.method(:stop))
  #
  #  Joyau.init(:my_module) # My module has been inited !
  #  Joyau.stop # My module has been stopped !
  #
  # If your module depends on something, you can specify the symbols of these:
  #  Joyau::Init.register(:mod, init_func, stop_func, :dep1, :dep2, ...)
  #
  module Init
    class Module # :nodoc:
      def initialize(init_func, stop_func, deps)
        @init_func = init_func
        @stop_func = stop_func

        @dependencies = deps

        @initialized = false
      end

      def init
        unless @initialized
          @init_func.call
          @initialized = true
        end
      end

      def stop
        if @initialized
          @stop_func.call
          @initialized = false
        end
      end

      def initialized?
        @initialized
      end

      attr_reader :dependencies
    end

    @modules = {}

    class << self
      # Inits the given module.
      def init(*modules)
        begin
          modules.each do |mod|
            unless @modules[mod].initialized?
              @modules[mod].dependencies.each { |dep| init(dep) }
              @modules[mod].init 
            end
          end
        rescue NoMethodError
          raise UndefinedModule, 
          "one of the specified module or one of its dependencies does not exist"
        end
      end

      # Stops the given modules, or all the module if no module name is given.
      def stop(*args)
        begin
          if args.size == 0
            @modules.each { |mod| mod.stop }
          else
            args.each { |mod| @modules[mod].stop }
          end
        rescue NoMethodError
          raise UndefinedModule, "one of the specified module does not exist"
        end
      end

      # Registers a new module
      def register(symbol, init_func, stop_func, *deps)
        @modules[symbol] = Init::Module.new(init_func, stop_func, deps)
      end
    end

    register :lib, Joyau.method(:initLib), Joyau.method(:stopLib)
    register :gfx, Joyau.method(:initGfx), Joyau.method(:stopGfx), :lib
    register :wlan, Joyau::Wlan.method(:init), Joyau::Wlan.method(:stop)
    register :intrafont, Joyau::Intrafont.method(:init), 
                         Joyau::Intrafont.method(:stop),
                         :gfx
    register :audio, Joyau.method(:initAudio), Joyau.method(:stopAudio)
    register :usb, Joyau::Usb.method(:init), Joyau::Usb.method(:stop)
  end

  class << self
    def init(*modules)
      Init.init(*modules)
    end
    
    def stop(*args)
      Init.stop(*args)
    end
  end
end
