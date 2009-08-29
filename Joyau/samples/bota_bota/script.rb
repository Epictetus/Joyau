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

bg = Sprite.new("bg.png")

hero = Hero.new
hero.setPicture("pic.png")
hero.autoDir = true

advs = []
for i in 1..4
  adv = Ghost.new
  if i % 2 == 0 # 2 ghosts, 2 solids. It's alright so.
    adv.picture = "ghost.png"
    adv.alpha = 127
  else
    adv.picture = "solid.png"
  end
  adv.setPos(rand(480), rand(272)) # Since there's no obstacle on the map...
  advs << adv
end

setTextBackground color(0, 0, 0, 0) 
# Two things : It'll work faster ( look at the oslib documentation ), 
# and it'll be ... Well, try with a black color as background. You'll understand.

skip = false

while mayPlay and $life > 0
  # Just calling the play and draw method of everyone...
  hero.play(advs)
  advs.each { |adv| adv.play(hero) }

  if !skip
    startDraw
    bg.draw
  
    advs.each { |adv| adv.draw(hero) }
    hero.draw
    
    drawText(0, 0, "Score : " + $score.to_s)
    drawText(0, 10, "Life : " + $life.to_s)
    endDraw
  end

  skip = sync
end

stopGfx
stopLib
