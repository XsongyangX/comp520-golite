/* Simulates a metro system and its incident reporting */
package main





type (
	Metro string
	Line []Metro
	Train struct{
		departureTime int
		line Line
	}
	System struct {
		trains []Train
		lines []Line
	}
	Incident struct {
		message string
		location Line
		station Metro
		time int
	}
)

func initialize() System{
	
	var (
		m1, m2, m3 Metro = Metro("The mountain"), Metro("The waterfall"), Metro("The field")
		n1, n2, n3, n4 Metro = Metro("Downtown"), Metro("Suburb"), Metro("Outskirts"), Metro("Port")
		mLine Line
		nLine Line
		lines []Line
		
		train1 Train
		train2 Train
		trains []Train
		
		sys System
	)

	// create the lines
	mLine = append(mLine, m1)
	mLine = append(mLine, m2)
	mLine = append(mLine, m3)
	nLine = append(nLine, n1)
	nLine = append(nLine, n2)
	nLine = append(nLine, n3)
	nLine = append(nLine, n4)
	lines = append(lines, mLine)
	lines = append(lines, nLine)
	
	// create the trains
	train1.departureTime = 0
	train1.line = mLine
	train2.departureTime = 5
	train2.line = nLine
	trains = append(trains, train1)
	trains = append(trains, train2)
	
	// form the system
	sys.trains = trains
	sys.lines = lines
	
	return sys
}

func createIncident(message string, location Line, station Metro, time int) Incident{
	var problem Incident
	problem.message = message
	problem.location = location
	problem.station = station
	problem.time = time
	
	return problem
}

func report(incident Incident) {
	print("Incident because of \"" + incident.message + "\"\n" +
	" located on the line belonging to \"" + string(incident.location[0]) + "\"\n" +
	" at station \"" + string(incident.station) + "\"\n" +
	" at time: ")
	println(incident.time)
}

func main() {
	sys := initialize()
	problem := createIncident("cellphone on the tracks", sys.lines[0], sys.lines[0][1], 3)
	report(problem)
	println("Hello, playground")
}
