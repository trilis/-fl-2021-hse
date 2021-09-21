states vertices:
	beginning start
	terminal zero
	terminal sink
	terminal divided5
	notdivided5

links zero_sink:
	start [0]> zero
	zero [0-9]> sink
	sink [0-9]> sink

links:
	start [1-46-9]> notdivided5
	start [5]> divided5

	notdivided5 [1-46-9]> notdivided5
	notdivided5 [05]> divided5

	divided5 [1-46-9]> notdivided5