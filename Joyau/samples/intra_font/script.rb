# -*- coding: utf-8 -*-
require 'joyau/old'

initLib
initGfx
Intrafont.init

txt = IntraText.new
txt.text = "Mon ouïe"
txt.load("flash0:/font/ltn7.pgf", 0)
txt.setStyle(0.8, 0xFFFFFF00, 0, 0)
txt.encoding = Intrafont::STRING_UTF8
txt.move(100, 100)

bg = Sprite.new("bg.png")

startDraw
bg.draw
endDraw

sync

while mayPlay
  startDraw
  bg.draw
  txt.activate
  txt.draw
  endDraw
  
  sync
end

Intrafont.stop
stopGfx
stopLib
