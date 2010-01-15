p (1..10000).inject(BigDecimal.new("0")) { |sum, i| 
  sum + BigDecimal.new("0.0001") 
}
