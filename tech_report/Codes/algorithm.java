/* speaker verification */
if(alize.matches(label,speech_1)) {
	eSpeak.calls(julius.listen());
} else {
	eSpeak.calls(alize.listen());
}

/* speech recognition  */
if(julius.matches(speech_2)){
	eSpeak.execute(some_command);
} else {
	eSpeak.calls(julius.listen());
}
