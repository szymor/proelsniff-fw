fsm = require "ppfsm"

function assert_generator(compare)
  return function (num)
    if num == compare then
	  print("[PASSED] Flat "..num)
	else
	  print("[FAILED] Got "..num..", should be "..compare)
	end
  end
end

function csv_line_parser(ts, val)
  timestamp = ts * 1000000
  value = tonumber(val)
  fsm.push_event(value == 1, timestamp)
end

fsm.reset()
fsm.flatcb = assert_generator(44)
csv = io.open("testdata/basic.csv", "r")
for line in csv:lines() do
  string.gsub(line, "([%d%.]+),%s*(%d)", csv_line_parser)
end

fsm.reset()
fsm.flatcb = assert_generator(44)
csv = io.open("testdata/double_ringbell.csv", "r")
for line in csv:lines() do
  string.gsub(line, "([%d%.]+),%s*(%d)", csv_line_parser)
end
