initLib
initGfx

sprite = Sprite.new("sprite.png")

while mayPlay
  startDraw
  sprite.draw
  endDraw
  
  sync
end

stopGfx
stopLib
