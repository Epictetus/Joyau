require 'joyau/init'
require 'joyau/color'

class MyModule
  def self.init; puts "My module has been initialized"; end
  def self.stop; puts "My module has been stopped"; end
end

Joyau::Init.register(:my_module, MyModule.method(:init), MyModule.method(:stop),
                     :gfx)

Joyau.init(:my_module)
Joyau.draw { |buffer| buffer.clear(Joyau::Color::RED) } while Joyau.mayPlay
Joyau.stop
