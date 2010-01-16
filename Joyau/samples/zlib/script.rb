data = File.read('pic.png')
puts data.size

data = Zlib::Deflate.deflate(data)
puts data.size

File.delete 'pic.png'
File.open('pic.png', 'w') do |file|
  file.write Zlib::Inflate.inflate(data)
end

puts "Done !"
