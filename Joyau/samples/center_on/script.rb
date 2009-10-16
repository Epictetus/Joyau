require 'joyau/old'

initLib
initGfx

bg = GameMap.new("obs_tile.png")
bg.setTileSize(32, 32)
bg << Tile.new(0, 0, 32, 32, 0)

bg.move(-70, -70)
bg.centerOn(32, 32)

while mayPlay
  startDraw
  bg.draw
  endDraw
  
  sync
end

stopGfx
stopLib
