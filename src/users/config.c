#include <asf.h>
#include "config.h"

extern void main_adc_init(void)
{
	/* ADC module configuration structure */
	struct adc_config adc_conf;
	/* ADC channel configuration structure */
	struct adc_channel_config adcch_conf;

	/* Configure the ADC module:
	 * - unsigned, more than 12-bit results
	 * - VCC/2  voltage reference
	 * - 200 kHz maximum clock rate
	 * - freerun conversion triggering
	 * - enabled internal temperature sensor
	 */
	adc_read_configuration(&ADCA, &adc_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
			ADC_REF_AREFD);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	
	adc_write_configuration(&ADCA, &adc_conf);
	
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adcch_enable_averaging(&adcch_conf, ADC_SAMPNUM_16X);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN3, ADCCH_NEG_NONE, 1);
	adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
	adcch_disable_interrupt(&adcch_conf);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
	
	/* Enable ADC which starts the freerun conversion.*/
	adc_enable(&ADCA);
	
	/* Do useful conversion */
	adc_start_conversion(&ADCA, ADC_CH0);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
}


extern void main_adc_correction_start(void)
{
	
	struct adc_channel_config adcch_conf;
	static bool correction_measures_done = false;
	static uint16_t offset_correction;	
	const uint16_t expected_value= 4095;	
	static uint16_t captured_value;
	static int16_t y1,y2;
	static float multi;
	

	if (correction_measures_done) {
//		goto main_adc_correction_enable;
	}

	main_dac_output(1000);
	adc_start_conversion(&ADCA, ADC_CH0);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	y1 = adc_get_unsigned_result(&ADCA, ADC_CH0);
	main_dac_output(2000);
	adc_start_conversion(&ADCA, ADC_CH0);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	y2 = adc_get_unsigned_result(&ADCA, ADC_CH0);
	multi = (float)(y2-y1)/1000.0;
	offset_correction = 2*y1-y2;
	captured_value = (int16_t)(multi*4095.0);
	printf("y1 %d,   ", y1);
	printf("y2 %d,   ", y2);
	printf("Offset correction %d,   ", offset_correction);
	printf("captured_value %d\n,   ", captured_value);
/*	
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adcch_enable_correction(&adcch_conf, offset_correction, 1, 1);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
*/	
	
	
//main_adc_correction_enable:
	  
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adcch_enable_correction(&adcch_conf, offset_correction, expected_value,
			captured_value);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
	
}


extern void adc_tempsense_select(void)
{
	/* ADC channel configuration structure */
	struct adc_channel_config adcch_conf;
	struct adc_config adc_conf;
	
	/* Configure ADC channel 0:
	 * - single-ended measurement from temperature sensor
	 * - interrupt flag set on completed conversion
	 */
	
	adc_disable(&ADCA);
	/* Configure the ADC module:
	 * - unsigned, more than 12-bit results
	 * - VCC /2 voltage reference
	 * - 200 kHz maximum clock rate
	 * - freerun conversion triggering
	 * - enabled internal temperature sensor
	 */
	adc_read_configuration(&ADCA, &adc_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
			ADC_REF_BANDGAP);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN, 1, 0);
	adc_enable_internal_input(&adc_conf, ADC_INT_TEMPSENSE);
	adc_write_configuration(&ADCA, &adc_conf);
	
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adcch_enable_averaging(&adcch_conf, ADC_SAMPNUM_2X);
	adcch_set_input(&adcch_conf, ADCCH_POS_TEMPSENSE, ADCCH_NEG_NONE, 1);
	adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
	adcch_disable_interrupt(&adcch_conf);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
	
	/* Enable ADC which starts the freerun conversion.*/
	adc_enable(&ADCA);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
}
extern void adc_adcch3_select(void)
{
	/* ADC channel configuration structure */
	struct adc_channel_config adcch_conf;
	struct adc_config adc_conf;
	
	adc_disable(&ADCA);	
	
	adc_read_configuration(&ADCA, &adc_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
	ADC_REF_AREFD);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN, 1, 0);	
	adc_write_configuration(&ADCA, &adc_conf);	
	
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adcch_enable_averaging(&adcch_conf, ADC_SAMPNUM_2X);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN3, ADCCH_NEG_NONE, 1);
	adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
	adcch_disable_interrupt(&adcch_conf);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
	
	/* Enable ADC which starts the freerun conversion.*/
	adc_enable(&ADCA);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
}

extern void adc_adcch5_select(void)
{
	/* ADC channel configuration structure */
	struct adc_channel_config adcch_conf;
	struct adc_config adc_conf;
	
	adc_disable(&ADCA);
	
	adc_read_configuration(&ADCA, &adc_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
	ADC_REF_AREFD);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_FREERUN, 1, 0);	
	adc_write_configuration(&ADCA, &adc_conf);
	
	adcch_read_configuration(&ADCA, ADC_CH0, &adcch_conf);
	adcch_enable_averaging(&adcch_conf, ADC_SAMPNUM_2X);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN5, ADCCH_NEG_NONE, 1);
	adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
	adcch_disable_interrupt(&adcch_conf);
	adcch_write_configuration(&ADCA, ADC_CH0, &adcch_conf);
	
	/* Enable ADC which starts the freerun conversion.*/
	adc_enable(&ADCA);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
	adc_wait_for_interrupt_flag(&ADCA, ADC_CH0);
}

extern void main_dac_init(void)
{
	/* DAC module configuration structure */
	struct dac_config dac_conf;

	/* Create configuration:
	 * - AVCC as reference
	 * - right adjusted channel value
	 * - both DAC channels active on :
	 *   - DAC0 (PA2 pin) for ADC V+
	 *   - DAC1 (PA3 pin) for ADC V-
	 * - manually triggered conversions on both channels
	 */
	dac_read_configuration(&DACA, &dac_conf);
	dac_set_conversion_parameters(&dac_conf, DAC_REFSEL_AVCC_gc, DAC_ADJ_RIGHT);
	dac_set_active_channel(&dac_conf, DAC_CH0, 0);
	dac_set_conversion_trigger(&dac_conf, 0, 0);
	dac_write_configuration(&DACA, &dac_conf);
	dac_enable(&DACA);
}

extern void main_dac_output(int16_t volt)
{
	/* Samples of values used:
	 * | ADC0 |    V+   | ADC1 |    V-   | V+ - V- |
	 * ---------------------------------------------
	 * | 4096 |   Vcc   |    0 |    0V   |   Vcc   |
	 * | 3072 | Vcc 3/4 | 1024 | Vcc 1/4 |  Vcc/2  |
	 * | 2048 | Vcc 1/2 | 2048 | Vcc 1/2 |    0V   |
	 * | 1024 | Vcc 1/4 | 3072 | Vcc 3/4 | -Vcc/2  |
	 * |    0 |    0V   | 4096 |   Vcc   |  -Vcc   |
	 */
	dac_wait_for_channel_ready(&DACA, DAC_CH0);
	dac_set_channel_value(&DACA, DAC_CH0, volt);	
	dac_wait_for_channel_ready(&DACA, DAC_CH0);
}