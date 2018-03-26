function receive (prod)
  local status, value = coroutine.resume(prod)
	return value
end
    
function send (x)
	coroutine.yield(x)
end

function producer ()
	return coroutine.create(function ()
		while true do
			local x = io.read()     -- produce new value
			send(x)
		end
	end)
end
    
function filter (prod)
	return coroutine.create(function ()
		-- amount of numbers seen in the stream
		local amount = 0
		local list={}
		while true do
			local last = receive(prod)   -- get new value
			-- if it is first number, set the three variables as number
      if type(last) == 'number' then
  			if amount==0 then
  				mini = last
  				maxi = last
  				average = last
  				median = last
  			end
  			amount=amount+1
  			table.insert( list, last )
  			if amount>1 then
  				-- possibly update maximum
  				if last > maxi then
  					maxi = last
  				end
  				-- possibly update minimum
  				if last < mini then
  					mini = last
  				end
  				-- calculate the average based on the last one, the amount of numbers, and the input
  				average = (average*(amount-1)+last)/amount
  				-- calculate median
  				table.sort( list )
  				if math.fmod(amount,2) == 0 then
  					median = ( list[amount/2] + list[(amount/2)+1] ) / 2
  				else
  					median = list[math.ceil(amount/2)]
  				end
  			end
  			-- create the string that will be printed
  			local prnt = string.format("Max: %f, Min: %f, Average: %f, Median: %f",maxi ,mini,average,median)
  			send(prnt)      -- send it to consumer
    else
      send(nil)
    end
  end
end)
end
    
function consumer (prod)
  while true do
    local x = receive(prod)
    -- get new value
    if x~= nil then
      io.write(x, "\n")          -- consume new value
    else
      io.write( "\n")
    end
end
end

-- start the program
consumer(filter(producer()))