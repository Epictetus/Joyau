# -*- coding: utf-8 -*-

initLib
initGfx
Intrafont.init

txt = IntraText.new
txt.text = "Mon ouïe"
txt.load("flash0:/font/ltn4.pgf", Intrafont::CACHE_ALL)
txt.setStyle(1, 0xFFFFFFFF, 0xFF000000, Intrafont::ALIGN_LEFT)
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
  txt.setStyle(1, 0xFFFFFFFF, 0xFF000000, Intrafont::ALIGN_LEFT)
  endDraw
  
  sync
end

Intrafont.stop
stopGfx
stopLib
