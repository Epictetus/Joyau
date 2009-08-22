initLib
initGfx

bg = GameMap.new("bg_tile.png", "obs_tile.png")
bg.pos = Point.new(0, 0)
bg.setTileSize(32, 32)

for i in 0..20
  bg << Tile.new(0, 0, 0, i * 32, 0)
  bg << Tile.new(0, 32, 0, i * 32, 32)
  bg << Tile.new(1, 0, 0, 0, 0)
  bg << Tile.new(1, 0, 0, 5 * 32, 5 * 32)
end

angel = Sprite.new("angel.png")
angel.setAnim(3, 4)

angel.direction = $directions["DOWN"]
angel.pos = Point.new(240 - angel.w / 2, 136 - angel.h / 2)

skip = false

while mayPlay
  readKeys

  if $keys["up"]
    bg.move(0, 2)
    angel.direction = $directions["UP"]
  end

  if $keys["down"]
    bg.move(0, -2)
    angel.direction = $directions["DOWN"]
  end
  
  if $keys["left"]
    bg.move(2, 0)
    angel.direction = $directions["LEFT"]
  end

  if $keys["right"]
    bg.move(-2, 0)
    angel.direction = $directions["RIGHT"]
  end

  tiles = bg.tiles.reject { |tile|
    angel.isOn(bg.x + tile.x, bg.y + tile.y) or
    angel.isOn(bg.x + tile.x + 32, bg.y + tile.y) or
    angel.isOn(bg.x + tile.x + 32, bg.y + tile.y + 32) or
    angel.isOn(bg.x + tile.x, bg.y + tile.y + 32)
  }
  bg.clearTiles
  bg << tiles

  if !skip
    startDraw
    clearScreen
    bg.draw
    angel.draw
    endDraw
  end
  
  skip = sync
end

stopGfx
stopLib
