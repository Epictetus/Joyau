# -*- coding: utf-8 -*-

require 'joyau/init'

Joyau.init(:gfx)

str = ""

while Joyau.mayPlay
  Joyau::Pad.update
  if Joyau::Pad.pressed? Joyau::Pad::CROSS
    str = Joyau::OSK.run(Joyau::OSK::ALL, Joyau.language, 
                         "Anonymous", "Input your name", 256) do
      Joyau.clearScreen
    end
  end

  break if Joyau::Pad.pressed? Joyau::Pad::SQUARE

  Joyau.draw do
    Joyau.clearScreen
    Joyau.drawScripted(0, 0, str)
  end
end

Joyau.stop
