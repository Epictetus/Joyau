# Almost exactly like one of the devlib sample :)

initLib
initGfx

menu = CircleMenu.new
menu.load("menu.png", 4, # icons 
          180, # distance 
          80 , # width
          80, # heigth
          40, # x
          240, # y
          20, # icon witdh
          120) # icon heigth

while mayPlay
  Pad.update

  if Pad.pressed? Pad::LEFT
    menu.changeSelection(1)
  elsif Pad.pressed? Pad::RIGHT
    menu.changeSelection(0)
  end

  startDraw
  clearScreen

  menu.draw
  drawText(0, 0, "Index : " + menu.index.to_s)

  endDraw

  sync
end

stopGfx
stopLib
