require 'joyau/buffer_library'
require 'joyau/enumerable'

def create_loader(regex, &block)
  loader = Module.new
  loader.instance_variable_set("@buffers", {})

  meta_loader = class << loader; self; end  
  meta_loader.module_eval do
    define_method("[]") do |res_name|
      if @buffers[res_name]
        @buffers[res_name]
      else
        create_buffer(res_name)
      end
    end

    define_method("create_buffer") do |res_name|
      filename = nil
      if res_name =~ regex
        filename = $1
      else
        raise Joyau::InvlidRessourceName, 
        "#{res_name} is not a valid ressource name"
      end

      @buffers[res_name] = Joyau::Buffer[filename]
      
       @buffers[res_name].lock do
        @buffers[res_name].each_with_pos do |x, y, col|
          @buffers[res_name][x, y] = block.call(x, y, col)
        end
      end

      return @buffers[res_name]
    end
  end

  Joyau::BufferLibrary.register_loader(loader, regex)
end

create_loader(/^gray:(.+)/) do |x, y, col|
  gray = (0.299 * col.r + 0.587 * col.g + 0.114 * col.b).to_i
  Joyau::Color.new(gray, gray, gray, col.a)
end

create_loader(/^red:(.+)/) do |x, y, col|
  Joyau::Color.new(col.r, 0, 0, col.a)
end

create_loader(/^green:(.+)/) do |x, y, col|
  Joyau::Color.new(0, col.g, 0, col.a)
end

create_loader(/^blue:(.+)/) do |x, y, col|
  Joyau::Color.new(0, 0, col.b, col.a)
end

Joyau.initLib
Joyau.initGfx

sprites = ["red", "green", "blue", "gray"].inject([]) do |ary, i|
  ary << Joyau::Sprite.from_res_name("#{i}:angel.png")
end

sprites.each_with_index do |sprite, i|
  sprite.setPos(sprite.w * i, 0)
end

while Joyau.mayPlay
  Joyau.draw do
    sprites.each { |sprite| sprite.draw }
  end
end

Joyau.stopGfx
Joyau.stopLib
