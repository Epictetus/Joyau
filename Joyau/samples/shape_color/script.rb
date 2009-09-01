initLib
initGfx

rect = DrawableRect.new # Of course, instanciate a Shape would have no sense..
rect.resize(100, 100)
rect.setColor(255, 0, 0)

skip = false

grad = [ color(255, 0, 0),
         color(0, 255, 0),
         color(0, 0, 255),
         color(255, 255, 255) ]

txt = DrawableText.new
txt.toggleScripted
txt.setPos(100, 0)

while mayPlay
  Pad.update
  if Pad.pressed? Pad::UP
    rect.setGradient grad
  elsif Pad.pressed? Pad::DOWN
    rect.setColor grad[rand(4)]
  end

  str = ""
  rect.getColors.each { |col|
    str = str + "r: " + col["r"].to_s + " " + 
    "g: " + col["g"].to_s + " " + 
    "b: " + col["b"].to_s + "\n"
  }
  txt.setText str

  if !skip
    startDraw
    clearScreen
    rect.draw
    txt.draw
    endDraw
  end

  skip = sync
end

stopGfx
stopLib
