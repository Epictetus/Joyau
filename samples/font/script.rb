require 'joyau/init'
require 'joyau/color'

Joyau.init(:intrafont)

font = Joyau::Font.new("flash0:/font/ltn10.pgf", 0)

font.set_style(1, Joyau::Color::WHITE, Joyau::Color::RED, 0)
font.encoding = Joyau::Intrafont::STRING_UTF8

Joyau.draw do |buffer|
  buffer.clear Joyau::Color::NONE

  font.print(0, 5, "Width of \"Hello world\" : #{font.width_of "Hello world"}")
  font.print(0, 5 + font.char_height, 
             "Height of each char : #{font.char_height}")
  
  font.print(0, 5 + font.char_height * 3, "Hello world !")
  font.print(Joyau::Rect.new(100, 100, 100, 100), 
             "V#{"e" * 7}ry l#{"o" * 7}ng t#{"e" * 7}xt")
end while Joyau.mayPlay

Joyau.stop
