# -*- coding: utf-8 -*-

Console2d.init
Console2d.switch_auto_flip

Console2d.print(26, 8, Console2d::LEFT,  "Hello world !")
Console2d.print(26, 9, Console2d::RIGHT, "Hello world !")

Console.wait_key(Console::ANY_KEY, "")
Console2d.clear

s = true
18.times { |i|
  Console2d.set_line_color(i, s ? 0xFF0000 : 0x00FF00)
  Console2d.print(26, i, s ? Console2d::LEFT : Console2d::RIGHT, "Hello world !")
  s = !s
}

Console.wait_key(Console::ANY_KEY, "")
Console2d.clear
Console2d.color = 0xFFFFFF;

Console2d.print(0, 0, Console2d::LEFT, "Dès Noël où un zéphyr haï")
Console.wait_key(Console::ANY_KEY, "")

Console2d.clear();
Console2d.switch_auto_flip

str = "ConsoleLib_2D ";
30.times { |count|
  Console2d.clear
  18.times { |i|
    Console2d.fill_line(Console2d::HORIZONTAL, i, 2 * count, str)
  }
  Console2d.flip
}
sleep 1

Console.wait_key(Console::ANY_KEY, "")
Console2d.clear

15.times { |i|
  Console2d.clear
  Console2d.print(0,   0, Console2d::LEFT,  (15 - i).to_s)
  Console2d.print(51,  0, Console2d::RIGHT, (15 - i).to_s)
  Console2d.print(0,  17, Console2d::LEFT,  (15 - i).to_s)
  Console2d.print(51, 17, Console2d::RIGHT, (15 - i).to_s);
  Console2d.flip
  sleep 1
}

Console2d.exit
