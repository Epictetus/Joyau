initLib
initGfx

begin
  text = DrawableText.new
  text.setColor(color(255, 0, 0, 255))
  text.setBackground(color(0, 0, 255, 255))
  
  text.setText("Hello world !")
  
  text.setPos(100, 100)
  
  while mayPlay
    startDraw
    text.draw
    endDraw
    
    sync
  end
  rescue
  File.open("error", "w") do |file|
    file.puts "Error:" + $!
  end
end  

stopGfx
stopLib
