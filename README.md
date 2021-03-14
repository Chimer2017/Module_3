# Module_3

## The Idea:

With the pandemic, many of us are spending exorbitant amounts of time at home. We sit in our rooms working, socializing, sleeping, etc. Yet, are we sure our environments are healthy as they can be? This was the inspiration for this project. Given that most of my classes are online, and I spend at least 6-8 hrs in my room every day, I wanted something that could tell me whether or not my environment is healthy and safe. At the same time, I didn't want some clunky sensor in the middle of my room. Thus, the idea for an artsy environment monitor was born.

## The Specifics:

With an idea in mind, it was time to nail down the specifics. I wanted something that would monitor the overall health and safety of my environment. After doing some research, the following metrics are most indicative of the healthiness of an environment: temperature, humidity, background noise, ambient light, and air quality. I decided to measure all of these metrics and would display the data through a dashboard of some sort. I also wanted the device to have an abstracted artful display. If an environment is healthy, there is a default light pattern on the device. If something is wrong, then the light display will flash red.

## How I Built It:

This project was built using several sensors to monitor the environment. It was then housed in a decorative box that wouldn't stick out like a sore thumb. The device was then connected to the internet where the data from the sensors was streamed to a live dashboard.

Materials:
1. SGP30 Air Quality Sensor
2. DHT 11 Temperature and Humidity Sensor
3. Adafruit Amplified Microphone Sensor
4. Light Potentiometer
5. ESP-32
6. Freenove Pixel LED
7. Battery (optional, this can be a wired/wireless device, all data is streamed over wifi)
8. Wires/Resistors as needed
9. Enclosure

### Hardware

The first step in this project was wiring up all the sensors to the ESP-32. All of this was relatively straightforward. Each sensor used in this project came with its own tutorial either from the Freenove tutorial or on the adafruit website. Once, every sensor was hooked up, I printed the output of the sensors to the serial monitor to make sure all the sensors were monitoring and responding to the environment. In this project, the hardware piece was rather straightforward.

Tutorial Links:

https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
https://learn.sparkfun.com/tutorials/sparkfun-air-quality-sensor---sgp30-qwiic-hookup-guide/all

Important Note: Some of these sensors did require soldering wires to the input contact points. Please follow safe soldering protocol if working with these sensors. During the soldering of the SGP30 sensor, I ended up having a bit of a hand spasm and some soldering got onto the logic circuitry itself. The air quality sensor outputted nonsensical data, and unfortunately had to be removed from the project.

### Software

This was the interesting part of this project. The software section of this project can be divided into two sections-- the first section focusing on how raw data was processed and transformed and the second section focusing on the datastream dashboard.

The microphone and light potentiometer both outputted data in rather nonsensical formats. It was an arbitrary range of values that correlated to the voltage differences measured by the senors for their respective mediums. The microphone data had to be processed and converted into decibels to be effectively interpretable. After some research, I found an algorithm for the sensor that converts that voltage value into decibels using a regression model. It requires inputting the average background noise as a starting point, and all measurements are compared to this default noise level. The exact algorithm can be seen in the audioCheck() function in the code. The link to the code repo can be found below. Regarding light, I was unable to find an effective algorithm to convert the values to lumens, so I took a similar regression approach. I recorded different values the senors read in dark and bright rooms and developed a decent range. It is, however, totally subjective, and if this device was ever commercialized, the light-sensing method would need to be greatly improved in accuracy and precision.

The next step was in streaming the data. The ESP-32 was connected to my home wifi network. I wanted to be able to check the exact metric of the health of my room from my laptop, so I decided to develop a dashboard. I found that adafruit has a dashboard tool called Adafruit IO. It makes it pretty easy to develop data stream dashboards. I made an account, got my unique key through which to send data, and set up multiple data feeds on the dashboard. Here is an example of what it looks like:

As you can see, I have specific feeds for each data stream as well as a graph that shows temperature and humidity over time. Since the dashboard is hosted online, you can access it here. However, it is probably not on at the moment and will be showing previous data.

The last piece of the software puzzle is the art aspect. The idea is for the device to be a decorative piece in a room. It will have an elegant light display. However, if something in the room environment is a miss, the light should begin to flash red. The red light will be a signal to check the dashboard and see what exactly is wrong. Here is an example of what the dashboard looks like when the room has an issue. (I had to yell at the sensor for a bit and cover the light sensor):

### Enclosure

Now, once the hardware and software were working, it was time to put everything into an enclosure. I used an old headphones box to create or at least attempt a modernist design for a decorative table piece. The pixel light was slipped under the top cover of the boxing giving the feel of an integrated system. Here are some videos:

 

Room is healthy:

https://youtu.be/xo_EitHileQ

The room is not healthy ( too loud and warm ):

https://youtu.be/wtoCa25EYT4

## Technical Obstacles

I had a few technical obstacles. First, soldering destroyed my air quality monitor. It would have been great to add AQI as a metric, but unfortunately, I have shaky hands and soldering is not my strong suit. As well, these aren't the highest quality sensors. Some of the data produced were at times questionable.

## In Conclusion

I was really proud of this project. It can be useful to have to lie around the house. Wouldn't you want to know whether your environment is healthy or not? Also, this project taught it is far more simple to prototype ideas than initially expected. Sometimes I have an idea, and I get discouraged because I think testing it is way out of the league in skills and knowledge. I thought this project was going to be rather difficult and complex, but in reality, there is a lot of support from the maker community, and the project came together. In an abstract sense, even some of the more complex ideas we have can probably be baseline tested using simple electronics and code which is incredible.
