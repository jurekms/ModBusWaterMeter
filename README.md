# ModBusWaterMeter
Nakładka na wodomierz zliczająca impulsy ,udostępniająca dane za pomocą protokołu MODBUS poprzez interfejs RS485.
Nakładka na wodomierz zliczająca ilość obrotów dwubarwnej tarczy znajdującej się na panelu odczytowym wodomierza. Optyczny układ przystawki oświetla tarczę obrotową wodomierza światłem IR, które jest odbijane (część lustrzana tarczy) lub pochłaniane (część czarna tarczy), odbita wiązka jest rejestrowana przez fototranzystor z którego sygnał analogowy kierowany jest do komparatora okienkowego zbudowanego na układzie LM393. Na wyjściu komparatora pojawia się sygnał wysoki - gdy fototranzystor jest oświetlony światłem odbitym od tarczy lub niski gdy światło jest pochłaniane przez tarczę i nie dociera do fototranzystora. Tak uformowany sygnał kierowany jest do wejścia D0 mikroprocesora 368. Oprogramowanie rejestruje dwa impulsy na każdy obrót tarczy - dwa przejścia lustro->czarny i czarny->lustro, co daje dwa impulsy na jeden litr przepływu.

szczegóły w opisie https://github.com/jurekms/ModBusWaterMeter/blob/main/WIKI/readme.docx


# ModBusWaterMeter
Pulse counting water meter overlay, providing data via the MODBUS protocol via the RS485 interface. Overlay on the water meter counting the number of revolutions of the two-color dial located on the reading panel of the water meter. The optical system of the attachment illuminates the rotating disc of the water meter with IR light, which is reflected (mirror part of the dial) or absorbed (black part of the dial), the reflected beam is recorded by the phototransistor from which the analog signal is directed to a window comparator built on the LM393 system. At the output of the comparator there is a high signal - when the phototransistor is illuminated by light reflected from the dial or low when the light is absorbed by the dial and does not reach the phototransistor. The signal formed in this way is directed to the D0 input of the microprocessor 368. The software records two pulses for each rotation of the dial - two mirror->black and black-> mirror transitions, which gives two pulses per one liter of flow.

more in https://github.com/jurekms/ModBusWaterMeter/blob/main/WIKI/readme.docx 
