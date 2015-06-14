void send2uno(char *stream)
{
	int i, count, limit=0;
	char rx;

	/* activate pins */
	gpio_export(tx_ctrl);
	gpio_export(tx_data);
	gpio_export(rx_ctrl);

	/* pinMode() like function */
	gpio_set_dir(tx_ctrl, OUTPUT_PIN);
	gpio_set_dir(tx_data, OUTPUT_PIN);
	gpio_set_dir(rx_ctrl, INPUT_PIN);

	for(i=0; i<strlen(stream); i++) {
		count = 0;
		/* BBB: Hey arduino, eu quero transmitir */
		gpio_set_value(tx_ctrl, HIGH);
		do {
			usleep(100);
			if(++count==30) // desiste apos 30 tentativas
				limit = 1;
		} while(!limit && (rx = gpio_get_value(rx_ctrl)) == '0'); 

		/* Tentei 30x e voce negou (rx='0'), então desisti */
		if(limit)
			break;

		/* BBB: To enviando */
		if(stream[i]=='0') {
			gpio_set_value(tx_data, LOW);
		} else {
			gpio_set_value(tx_data, HIGH);
		}

		/* BBB: Acabei de enviar */
		gpio_set_value(tx_ctrl, LOW);
		usleep(300);
	}

	/* deactivate pins */
	gpio_unexport(tx_ctrl);
	gpio_unexport(tx_data);
	gpio_unexport(rx_ctrl);
}
