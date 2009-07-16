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
  readKeys

  if $keys["pressed_left"]
    menu.changeSelection(1)
  elsif $keys["pressed_right"]
    menu.changeSelection(0)
  end

  startDraw
  clearScreen

  menu.draw
  drawText(0, 0, "Index : " + menu.getIndex.to_s)

  endDraw

  sync
end

stopGfx
stopLib
