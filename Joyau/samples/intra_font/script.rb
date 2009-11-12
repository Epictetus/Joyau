# -*- coding: utf-8 -*-

require 'joyau/color'

Joyau.initLib
Joyau.initGfx
Joyau::Intrafont.init

def rand_col
  Joyau::Color.new(rand(256), rand(256), rand(256))
end

def gen_text
  txt = []

  16.times { |i|
    txt << Joyau::IntraText.new
    txt[i].text = "Mon ouïe"
    txt[i].load("flash0:/font/ltn#{i}.pgf", Joyau::Intrafont::CACHE_MED)
    txt[i].setStyle(1, rand_col, rand_col, 0)
    txt[i].encoding = Joyau::Intrafont::STRING_UTF8
    txt[i].setPos(0, 15 * (i + 1))
  }

  txt << Joyau::IntraText.new
  txt[16].text = "もん うい"
  txt[16].load("flash0:/font/jpn0.pgf", Joyau::Intrafont::CACHE_MED)
  txt[16].setStyle(1, rand_col, rand_col, 0)
  txt[16].encoding = Joyau::Intrafont::STRING_UTF8
  txt[16].setPos(120, 20)

  txt << Joyau::IntraText.new
  txt[17].text = "한국말"
  txt[17].load("flash0:/font/kr0.pgf", Joyau::Intrafont::CACHE_MED)
  txt[17].setStyle(1, rand_col, rand_col, 0)
  txt[17].encoding = Joyau::Intrafont::STRING_UTF8
  txt[17].setPos(120, 40)

  return txt
end

txt = gen_text
while Joyau.mayPlay
  Joyau::Pad.update
  txt = gen_text if Joyau::Pad.pressed? Joyau::Pad::CROSS
  Joyau.startDraw
  Joyau.clearScreen
  txt.each { |i| i.draw }
  Joyau.endDraw

  Joyau.sync
end

Joyau::Intrafont.stop
Joyau.stopGfx
Joyau.stopLib
