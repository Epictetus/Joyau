# A full game in Ruby for the PSP !
# Enjoy :-)

require 'joyau/init'
require 'joyau/color'

require 'projectil'
require 'ghost'
require 'hero'

# We could start with 42 as score, and without any life. But why
# Not using a normal value ?
$score = 0
$life = 100

Joyau.init(:gfx)

bg = Joyau::Sprite.new("bg.png")

hero = Hero.new("pic.png")
hero.autoDir = true

advs = (1..4).inject([]) do |ary, i|
  if i % 2 == 0
    ary << Ghost.new("ghost.png")
    ary.last.alpha = 127
  else
    ary << Ghost.new("solid.png")
  end

  ary
end

advs.each { |adv| adv.setPos(rand(480), rand(272)) }

Joyau.setTextBackground Joyau::Color::NONE
# Two things : It'll work faster ( look at the oslib documentation ), 
# and it'll be ... Well, try with a black color as background. You'll understand.

while Joyau.mayPlay and $life > 0
  # Just calling the play and draw method of everyone...
  hero.play(advs)
  advs.each { |adv| adv.play(hero) }

  Joyau.draw do
    bg.draw
    
    advs.each { |adv| adv.draw(hero) }
    hero.draw

    Joyau.drawText(0, 0, "Score : " + $score.to_s)
    Joyau.drawText(0, 10, "Life : " + $life.to_s)
  end
end

Joyau.stop
