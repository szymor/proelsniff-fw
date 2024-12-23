-- Tencent Cloud MQTT server in China
mqtt_server = "81.69.199.239"

uid = node.chipid()
clientid = "proelsniff"..uid
topic = "proelsniff/"..uid.."/"

GPIO0 = 3
GPIO2 = 4
gpio.mode(GPIO0, gpio.OUTPUT)
gpio.write(GPIO0, gpio.HIGH)
gpio.mode(GPIO2, gpio.OUTPUT)
gpio.write(GPIO2, gpio.HIGH)

wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, function(T)
  print("IP: "..T["IP"])
  print("Netmask: "..T["netmask"])
  print("Gateway: "..T["gateway"])
  m = mqtt.Client(clientid, 120)
  m:on("connect", function(client)
    print("Connected to "..mqtt_server..".")
	local us_time = tmr.ccount() / node.getcpufreq()
	m:publish(topic.."setup_time", us_time, 1, 0)
	gpio.write(GPIO2, gpio.LOW)
  end)
  m:on("connfail", function(client)
    print("Connection to "..mqtt_server.." failed.")
  end)
  m:on("offline", function(client)
    print("MQTT client disconnected.")
	gpio.write(GPIO2, gpio.HIGH)
  end)
  print("Connecting to "..mqtt_server.."...")
  m:connect(mqtt_server)
end)

tmr.create():alarm(30000, tmr.ALARM_SINGLE, function()
  if wifi.sta.getip() == nil then
    print("Starting the Captive Portal...")
    enduser_setup.start()
  end
end)

print("MQTT server: "..mqtt_server)
print("Topic root: "..topic)

-- m:publish(topic.."flat", <flat_number_here>, 2, 0)
-- m:publish(topic.."idle_voltage", <idle_voltage_in_string_here>, 2, 0)
-- m:close()
