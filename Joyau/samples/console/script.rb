Console.init(Console::LITE)

Console.puts "Hello world !"

Console.text_color = 0x0000FF
Console.puts "Hello world in blue !"

Console.text_color = 0x00FF00
Console.puts "Hello world in green !"

Console.text_color = 0xFF0000
Console.puts "Hello world in red !"

Console.text_color = 0xFFFFFF

Console.wait_key(Console::ANY_KEY, "Press any key...")

5.times { 
  Console.print "."
  sleep 1
}

Console.exit
