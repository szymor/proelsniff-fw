local fsm = {}

fsm.STATE_IDLE = 0
fsm.STATE_RESET_START = 1
fsm.STATE_RESET_END = 2
fsm.STATE_FLAT_START = 3
fsm.STATE_FLAT_END = 4
fsm.STATE_RINGTONE = 5

function fsm.reset()
  fsm.state = fsm.STATE_IDLE
  fsm.last = 0
  fsm.flatcb = print
end

fsm.reset()

function fsm.push_event(raising, timestamp)
  period = timestamp - fsm.last
  if period < 0 then
    print("Something's wrong with time.")
	fsm.last = 0
  else
    fsm.last = timestamp
  end

  if fsm.state == fsm.STATE_IDLE then
    if not raising then
      fsm.state = fsm.STATE_RESET_START
    end
  elseif fsm.state == fsm.STATE_RESET_START then
    if raising and period > 200000 and period < 500000 then
      fsm.flat_number = 0
      fsm.state = fsm.STATE_RESET_END
    else
      fsm.state = fsm.STATE_IDLE
    end
  elseif fsm.state == fsm.STATE_RESET_END then
    if not raising and period > 180000 and period < 250000 then
      fsm.state = fsm.STATE_FLAT_START
    else
      fsm.state = fsm.STATE_IDLE
    end
  elseif fsm.state == fsm.STATE_FLAT_START then
    if raising and period > 5 and period < 30 then
      fsm.flat_number = fsm.flat_number + 1
      fsm.state = fsm.STATE_FLAT_END
    else
      fsm.state = fsm.STATE_IDLE
    end
  elseif fsm.state == fsm.STATE_FLAT_END then
    if not raising and period > 150 and period < 300 then
      fsm.state = fsm.STATE_FLAT_START
    elseif not raising and period > 30000 and period < 1000000 then
      if fsm.flatcb ~= nil then
        fsm.flatcb(fsm.flat_number)
      end
      fsm.state = fsm.STATE_RINGTONE
    else
      fsm.state = fsm.STATE_IDLE
    end
  elseif fsm.state == fsm.STATE_RINGTONE then
    if period > 5000000 then
      if not raising then 
        fsm.state = fsm.STATE_RESET_START
      else
        fsm.state = fsm.STATE_IDLE
      end
    end
  end
end

return fsm
