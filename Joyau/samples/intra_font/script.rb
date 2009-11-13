# -*- coding: utf-8 -*-

require 'joyau/color'

Joyau.initLib
Joyau.initGfx
Joyau::Intrafont.init

# Returns a random opaque color
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
  txt[16].setPos(200, 30)

  txt << Joyau::IntraText.new
  txt[17].text = "한국말"
  txt[17].load("flash0:/font/kr0.pgf", Joyau::Intrafont::CACHE_MED)
  txt[17].setStyle(1, rand_col, rand_col, 0)
  txt[17].encoding = Joyau::Intrafont::STRING_UTF8
  txt[17].setPos(200, 60)

  return txt
end

txt     = gen_text
index   = 0
general = true

while Joyau.mayPlay
  Joyau::Pad.update
  txt = gen_text if Joyau::Pad.pressed? Joyau::Pad::CROSS
  index += 1 if Joyau::Pad.pressed? Joyau::Pad::RIGHT and txt[index + 1]
  index -= 1 if Joyau::Pad.pressed? Joyau::Pad::LEFT and txt[index - 1]
  general = !general if Joyau::Pad.pressed? Joyau::Pad::SELECT
  
  Joyau.startDraw
  Joyau.clearScreen
  unless general
    old_pos = Joyau::Point.new(txt[index].x, txt[index].y)
    txt[index].setPos(200, 100)
    txt[index].draw
    txt[index].pos = old_pos
  else
    txt.each { |i| i.draw }
  end
  Joyau.endDraw

  Joyau.sync
end

Joyau::Intrafont.stop
Joyau.stopGfx
Joyau.stopLib
