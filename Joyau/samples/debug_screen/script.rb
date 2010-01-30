require 'joyau/color'

Joyau::Debug.init

Joyau::Debug.puts
Joyau::Debug.puts "Test !"

Joyau::Debug.bg_color = Joyau::Color::WHITE
Joyau::Debug.text_color = Joyau::Color::GREEN

Joyau::Debug.puts "This a text", "This is another one", 42
Joyau::Debug.puts "\\n\n"

Joyau::Debug.print 1, 2, 3, 4, 5, "\n"

Joyau::Debug.disable_background
Joyau::Debug.puts "\nThis is my world !"

sleep 3[A]
Joyau::Debug.clear
Joyau::Debug.puts "How strange !"

sleep 3
Joyau.exitGame
