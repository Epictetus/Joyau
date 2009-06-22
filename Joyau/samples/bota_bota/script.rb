# A full game in Ruby for the PSP !
# Enjoy :-)

load "projectil.rb"
load "ghost.rb"
load "hero.rb"

# We could start with 42 as score, and without any life. But why
# Not using a normal value ?
$score = 0
$life = 100

initLib
initGfx

bg = Sprite.new
bg.setPicture("bg.png")

hero = Hero.new
hero.setPicture("pic.png")

advs = []
for i in 1..4
  adv = Ghost.new
  if i % 2 == 0 # 2 ghosts, 2 solids. It's alright so.
    adv.setPicture("ghost.png")
    adv.setAlpha(127)
  else
    adv.setPicture("solid.png")
  end
  adv.setPos(rand(480), rand(272)) # Since there's no obstacle on the map...
  advs.push(adv)
end

no_color = Hash.new # I know I could initialize all in on line...
no_color["r"] = 0   # but it's still really easy to read so...
no_color["g"] = 0
no_color["b"] = 0
no_color["a"] = 0
setTextBackground(no_color) # Two things : It'll work speeder 
# ( look at the oslib documentation ), and it'll be ...Try with a black
# color as background. You'll understand.

while mayPlay and $life > 0
  # Just calling the play and draw method of everyone...
  hero.play(advs)
  for adv in advs
    adv.play(hero)
  end

  startDraw
  bg.draw
  for adv in advs
    adv.draw(hero)
  end
  hero.draw
  drawText(0, 0, "Score : " + $score.to_s)
  drawText(0, 10, "Life : " + $life.to_s)
  endDraw

  sync
end

stopGfx
stopLib
