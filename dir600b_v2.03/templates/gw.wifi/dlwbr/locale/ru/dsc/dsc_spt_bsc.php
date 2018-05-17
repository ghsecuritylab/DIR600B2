﻿<h1>МЕНЮ ПОДДЕРЖКИ</h1>
<table border=0 cellspacing=0 cellpadding=0 width=750>
<tr>
	<td height=40>
		<p><font size=4><strong>Установка</strong></font></p>
		<p>
			<a name=01><strong>Установка Интернет</strong></a><br>
			При настройке устройства впервые рекомендуется кликнуть по Мастеру установки Интернет-соединения и следовать появляющимся на экране инструкциям.
			При желании задать настройки Интернет-соединения вручную кликните по "Установка Интернет-соединения вручную"
		</p>
	</td>
</tr>
<tr>
	<td height=59>
		<p>
			<strong>Запуск Мастера установки Интернет-соединения </strong><br>
			Кликните по этой кнопке, чтобы с помощью появляющихся на экране несложных шагов настроить Интернет-соединение.<br>
			<strong>Запуск Мастера установки безопасного беспроводного соединения </strong><br>
			Кликните по этой кнопке, чтобы с помощью появляющихся на экране несложных шагов задать настройки беспроводной сети, в том числе безопасности.
		</p>
	</td>
</tr>
<tr><td height=32>&nbsp;</td></tr>
<tr>
	<td height=46>
		<p>
			<strong>Установка Интернет-соединения вручную</strong><br>
			Выберите эту опцию при желании ввести настройки для подключения маршрутизатора к Интернет.<br>
			<strong>Режим точки доступа</strong><br>
			Выбор &quot;Режим точки доступа&quot; приведет к работе устройства в качестве беспроводной точки доступа.
			Все функции NAT будут отключены, кроме беспроводных настроек.<br>
			<strong>Тип Интернет-соединения </strong><br>
			Настройки Интернет-соединения используются для подключения <?query("/sys/modelname");?>
			к Интернет. Информацию, которую необходимо ввести на данной странице, необходимо получить от провайдера.
			Эти настройки часто называются как &quot;публичные настройки&quot;.
			Пожалуйста, выберите соответствующую опцию для своего провайдера.
			При сомнениях в выборе нужной опции, пожалуйста, обратитесь к провайдеру.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=61>
		<p>
			<strong>Статический IP-адрес.</strong><br>
			Выберите эту опцию, если от провайдера был получен IP-адрес, Маска подсети, Шлюз по умолчанию и адрес DNS-сервера.
			Введите эту информацию в соответствующие поля.
			При сомнениях, какую информацию ввести, пожалуйста, обратитесь к своему провайдеру.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=142>
		<p>
			<strong>Динамический IP-адрес</strong><br>
			Выберите эту опцию, если провайдер назначает IP-адрес автоматически.
			При использовании кабельного модема обычно провайдеры используют динамическое назначение IP-адреса.
		</p>
		<p>
			<strong><em>Имя Хоста </em></strong> (дополнительно) -
			Поле Имя Хоста не является обязательным, но может потребоваться для настройки Интернет-соединения некоторых провайдеров.
			По умолчанию, в этом поле указано название модели маршрутизатора.<br>
			<strong><em>MAC-адрес </em></strong> (дополнительно) -
			Поле MAC-адрес необходимо для настройки Интернет-соединения некоторых провайдеров.
			По умолчанию, в данном поле содержится MAC-адрес WAN-интерфейса на <?query("/sys/modelname");?>.
			Можно использовать кнопку &quot;Клонировать MAC-адрес&quot;, чтобы автоматически скопировать MAC-адрес Ethernet-адаптера компьютера,
			используемого для настройки устройства. Это поле необходимо заполнить только в том случае, если это требуется для настройки Интернет-соединения данного провайдера.<br>
			<strong><em>Первичный DNS-адрес </em></strong> -
			Введите первичный IP-адрес DNS-сервера, полученный от провайдера.<br>
			<strong><em>Вторичный DNS-адрес </em></strong> (дополнительно) -
			Если от провайдера был получен IP-адрес вторичного  DNS-сервера, введите его в это поле.<br>
			<strong><em>MTU </em></strong> -
			MTU определяет максимальный размер пакета, который может быть отправлен по сети.
			Сообщения, размер которых превышает MTU, разбиваются на несколько пакетов.
			По умолчанию в данном поле установлено значение1500.
			Изменение данной настройки может неблагоприятно отразиться на производительности маршрутизатора.
			Поэтому это значение целесообразно менять только по рекомендации специалистов технической поддержки или провайдера.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=47>
		<p>
			<strong>PPPoE</strong><br>
			Выберите эту опцию, если провайдер использует для установки Интернет-соединения протокол PPPoE (Point to Point Protocol over Ethernet).
			Провайдеры DSL обычно используют эту опцию. Выберите Динамический PPPoE для автоматического получения IP-адреса для PPPoE-соединения (используется большинством PPPoE-соединений).
			Выберите Статический PPPoE, чтобы использовать статический IP-адрес для PPPoE-соединения.
		</p>
		<p>
			<strong><em>Имя пользователя </em></strong> - Введите Имя пользователя для PPPoE-соединения.<br>
			<strong><em>Пароль </em></strong> - Введите пароль для PPPoE-соединения.<br>
			<strong><em>Имя сервиса </em></strong> (дополнительно) -
			Если провайдер использует Имя сервиса для PPPoE-соединения, введите его в данном поле.<br>
			<strong><em>IP-адрес </em></strong> -
			Эта опция доступна только при выборе Статический PPPoE. Введите статический IP-адрес PPPoE-соединения.<br>
			<strong><em>MAC-адрес </em></strong> (дополнительно) -
			Поле MAC-адрес необходимо для настройки Интернет-соединения некоторых провайдеров.
			По умолчанию, в данном поле содержится MAC-адрес WAN-интерфейса на <?query("/sys/modelname");?>.
			Можно использовать кнопку &quot;Клонировать MAC-адрес&quot;, чтобы автоматически скопировать MAC-адрес
			Ethernet-адаптера компьютера, используемого для настройки устройства. Это поле необходимо заполнить только в том случае,
			если это требуется для настройки Интернет-соединения данного провайдера.<br>
			<strong><em>Первичный DNS-адрес </em></strong> -
			Введите IP-адрес первичного DNS-сервера, полученный от провайдера.<br>
			<strong><em>Вторичный DNS-адрес </em></strong> (дополнительно) -
			Если от провайдера был получен IP-адрес вторичного  DNS-сервера, введите его в это поле.<br>
			<strong><em>Maximum Idle time </em></strong> -
			Время неактивности устройства перед завершением PPPoE-сессии.
			Введите Maximum Idle Time (в минутах), чтобы определить максимальный период времени сохранения Интернет-соединения в случае неактивности устройства.
			Если соединение неактивно в течение Maximum Idle Time, то оно будет разорвано. Это опция доступна только в режиме "Соединение по требованию".<br>
			<strong><em>MTU </em></strong> -
			MTU определяет максимальный размер пакета, который может быть отправлен по сети.
			Сообщения, размер которых превышает MTU, разбиваются на несколько пакетов.
			По умолчанию в данном поле установлено значение1500.
			Изменение данной настройки может неблагоприятно отразиться на производительности маршрутизатора.
			Поэтому это значение целесообразно менять только по рекомендации специалистов технической поддержки или провайдера.<br>
			<strong><em>Выбор режима подключения</em></strong> -
			Выберите "Всегда включен", чтобы маршрутизатор не разрывал PPPoE-сессию.
			Выберите Вручную, если хотите контролировать, когда маршрутизатор подключается или отключается от Интернет.
			Опция Соединение по требованию позволяет маршрутизатору установить Интернет-соединение только в том случае,
			если устройство в сети пытается получить доступ к ресурсам в Интернет.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>PPTP</strong><br>
			Выберите эту опцию, если провайдер использует протокол  PPTP (Point to Point Tunneling Protocol) для настройки соединения и назначил Имя пользователя для доступа к Интернет.
			Выберите Динамический PPTP для получения IP-адреса автоматически для PPTP-соединения.
			Выберите Статический PPTP, чтобы использовать статический IP-адрес для PPTP-соединения.
		</p>
		<p>
			<strong><em>IP-адрес </em></strong> - Введите IP-адрес, назначенный провайдером.<br>
			<strong><em>Маска подсети </em></strong> - Введите маску подсети, назначенную провайдером.<br>
			<strong><em>Шлюз</em></strong> - Введите IP-адрес шлюза, назначенный провайдером.<br>
			<strong><em>DNS - </em></strong>Введите IP-адрес DNS-сервера, назначенный провайдером.<br>
			<strong><em>IP-адрес сервера </em></strong> -
			Введите IP-адрес сервера, назначенный провайдером.<br>
			<strong><em>Учетная запись </em></strong> - Введите Имя пользователя PPTP.<br>
			<strong><em>Пароль </em></strong> - Введите пароль PPTP.<br>
			<strong><em>Maximum Idle time </em></strong> -
			Время неактивности устройства перед завершением PPTP-сессии. Введите Maximum Idle Time
			(в минутах), чтобы определить максимальный период времени сохранения Интернет-соединения в случае неактивности устройства.
			Если соединение неактивно в течение Maximum Idle Time, то оно будет разорвано.
			Это опция доступна только в режиме "Соединение по требованию".<br>
			<strong><em>MTU </em></strong> -
			MTU определяет максимальный размер пакета, который может быть отправлен по сети. Сообщения, размер которых превышает MTU,
			разбиваются на несколько пакетов. По умолчанию в данном поле установлено значение1500. Изменение данной настройки может
			неблагоприятно отразиться на производительности маршрутизатора. Поэтому это значение целесообразно менять только по
			рекомендации специалистов технической поддержки или провайдера.<br>
			<strong><em>Выбор режима подключения</em></strong> -
			Выберите "Всегда включен", чтобы маршрутизатор не разрывал PPPoE-сессию.
			Выберите Вручную, если хотите контролировать, когда маршрутизатор подключается или отключается от Интернет.
			Опция Соединение по требованию позволяет маршрутизатору установить Интернет-соединение только в том случае,
			если устройство в сети пытается получить доступ к ресурсам в Интернет.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>L2TP </strong><br>
			Выберите эту опцию, если провайдер использует протокол  L2TP и назначил Имя пользователя и пароль для доступа к Интернет.
			Выберите Динамический L2TP, чтобы автоматически получить IP-адрес для L2TP-соединия.
			Выберите Статический L2TP, чтобы использовать статический IP-адрес для L2TP-соединения.
		</p>
		<p>
			<strong><em>IP-адрес </em></strong> - введите IP-адрес, назначенный провайдером.<br>
			<strong><em>Маска подсети </em></strong> - Введите маску подсети, назначенную провайдером.<br>
			<strong><em>Шлюз</em></strong> - Введите IP-адрес шлюза, назначенный провайдером.<br>
			<strong><em>DNS</em></strong> - Введите IP-адрес DNS-сервера, назначенный провайдером.<br>
			<strong><em>IP-адрес сервера </em></strong> -
			Введите IP-адрес сервера, назначенный провайдером.<br>
			<strong><em>Учетная запись L2TP </em></strong> - Введите Имя пользователя L2TP.<br>
			<strong><em>Пароль L2TP </em></strong> - Введите пароль L2TP.<br>
			<strong><em>Maximum Idle time </em></strong> -
			Время неактивности устройства перед завершением L2TP-сессии. Введите Maximum Idle Time (в минутах),
			чтобы определить максимальный период времени сохранения Интернет-соединения в случае неактивности устройства.
			Если соединение неактивно в течение Maximum Idle Time, то оно будет разорвано.
			Это опция доступна только в режиме "Соединение по требованию".<br>
			<strong><em>MTU</em></strong> -
			MTU определяет максимальный размер пакета, который может быть отправлен по сети.
			Сообщения, размер которых превышает MTU, разбиваются на несколько пакетов.
			По умолчанию в данном поле установлено значение1500. Изменение данной настройки может неблагоприятно отразиться на производительности маршрутизатора.
			Поэтому это значение целесообразно менять только по рекомендации специалистов технической поддержки или провайдера.
			<strong><em>Выбор режима подключения</em></strong> -
			Выберите "Всегда включен", чтобы маршрутизатор не разрывал PPPoE-сессию. Выберите Вручную, если хотите контролировать,
			когда маршрутизатор подключается или отключается от Интернет. Опция Соединение по требованию позволяет маршрутизатору установить Интернет-соединение только в том случае,
			если устройство в сети пытается получить доступ к ресурсам в Интернет.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>PPTP (Dual Access)</strong><br>
			Настройки Интернет соединения PPTP (Dual Access) аналогичны настроенным ранее PPTP-соединениям.
			При необходимости установить статический маршрут провайдером, пожалуйста,
			обратитесь к функции &quot;Маршрутизация&quot; в меню &quot;РАСШИРЕННЫЕ НАСТРОЙКИ&quot; для последующих настроек.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>PPPoE (Dual Access)</strong><br>
			Некоторые PPPoE-соединения используют статический IP-маршрут в дополнение к глобальным IP-настройкам соединения.
			Поэтому требуется дополнительный шаг, чтобы определить IP-настройки физического WAN-порта.
			Настройки Интернет-соединения PPPoE (Dual Access) аналогичны описанным ранее настройкам PPPoE-соединения и добавить IP-настройки WAN, требуемые провайдером.
			Если необходимо установить статический маршрут провайдером, пожалуйста, обратитесь к функции &quot;Маршрутизация&quot;
			в меню &quot;Расширенные настройки&quot; для последующей настройки.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<a name=02><strong>Настройка беспроводного доступа</strong></a><br>
			Страница Установка беспроводного доступа содержит настройки для режима точки доступа <?query("/sys/modelname");?>.
			Эта страница позволяет настроить беспроводную сеть под нужды заказчика или настроить <?query("/sys/modelname");?>
			для настройки установленной ранее беспроводной сети.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>Wi-Fi Protected Setup (Другое название - WCN 2.0 для Windows Vista)</strong><br>
			Эта функция предоставляет интуитивно понятный способ настройки безопасности беспроводного доступа.
			Предлагается два возможных формата: PIN-номер и push button. Ввод PIN-номера, содержащийся в утилите
			беспроводного адаптера или утилите беспроводного клиента Windows Vista, если беспроводный адаптер имеет сертифицированный Windows Vista драйвер,
			позволяет автоматически настроить функции беспроводной безопасности между маршрутизатором и клиентом.
			Беспроводный адаптер должен поддерживать Wi-Fi Protected Setup того же формата, чтобы получить все преимущества данной функции.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>Имя беспроводной сети</strong><br>
			Это имя беспроводной локальной сети (WLAN), также известное как SSID (Service Set Identifier).
			По умолчанию SSID <?query("/sys/modelname");?> &quot;dlink&quot;.
			Эту настройку легко поменять для установки новой беспроводной сети или добавления <?query("/sys/modelname");?> в существующую беспроводную сеть.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>Беспроводный канал</strong><br>
			Указывает, какой канал <?query("/sys/modelname");?> используется для работы. По умолчанию задан 6.
			Эта настройка может быть изменена для соответствия настройкам существующей сети или настройки новой беспроводной сети.
			Поставьте галочку в поле Включить автоматическое сканирование, чтобы <?query("/sys/modelname");?> автоматически выбрал нужный канал для работы.
			Рекомендуется использовать эту опцию, поскольку маршрутизатор автоматически выберет канал с наименьшей интерференцией.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>WMM</strong><br>
			Выберите опцию Включить, чтобы включить QoS для беспроводного интерфейса <?query("/sys/modelname");?>.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>Включить Hidden Wireless </strong><br>
			Выберите опцию Включено, если не нужно передавать SSID
			<?query("/sys/modelname");?> в широковещательном режиме по сети.&nbsp;При выборе опции Включено SSID <?query("/sys/modelname");?>
			не будет виден через утилиту Site Survey, поэтому при настройке беспроводных клиентов необходимо будет
			знать SSID <?query("/sys/modelname");?> и ввести его вручную для подключения к маршрутизатору.
			Эта опция включена по умолчанию.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>Режим беспроводной безопасности</strong><br>
			Обеспечение безопасности беспроводной сети очень важно, поскольку позволяет обеспечить сохранность информации, передаваемой по беспроводной сети.
			<?query("/sys/modelname");?> поддерживает 4 варианта беспроводной безопасности:
			WEP, только WPA, только WPA2 и WPA/WPA2 (автоматическое обнаружение)
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>WEP </strong><br>
			Wired Equivalent Protocol (WEP) является протоколом обеспечения беспроводной безопасности для беспроводных локальных сетей (WLAN).
			WEP обеспечивает безопасность, шифруя данные, передаваемые в WLAN. <?query("/sys/modelname");?> поддерживает
			2 варианта WEP-шифрования: 64-битное и 128-битное. По умолчанию шифрование WEP выключено.
			Настройки WEP доступны для изменения для соответствия настройкам существующей сети или настройки беспроводной сети.
		</p>
		<p>
			<strong><em>Аутентификация</em></strong> -
			Аутентификация представляет собой процесс, с помощью которого <?query("/sys/modelname");?> проверяет подлинность сетевых устройств,
			пытающихся подключиться к беспроводной сети. При использовании WEP выделяется два типа аутентификации.<br> 
			<strong><em>Открытая система</em></strong> -
			Выберите эту опцию, чтобы разрешить всем беспроводным устройствам взаимодействие с <?query("/sys/modelname");?> до того,
			как их попросят ввести ключ, необходимый для получения доступа к сети.<br>
			<strong><em>Shared Key</em></strong> -
			Выберите эту опцию, чтобы любое беспроводное устройство, пытающееся взаимодействовать с <?query("/sys/modelname");?>,
			предоставляло ключ шифрования, необходимый для доступа к сети, до того, как ему будет разрешено взаимодействовать с
			<?query("/sys/modelname");?>.<br>
			<strong><em>Шифрование WEP</em></strong> -
			Выберите один из вариантов шифрования WEP, который будет использоваться в сети. Доступны две опции WEP-шифрования 64-битное и 128-битное.<br>
			<strong><em>Тип ключа</em></strong> -
			<?query("/sys/modelname");?> поддерживает такие типы ключей, как HEX (шестнадцатеричная) и ASCII (Американский стандартный
			код для обмена информацией). Тип ключа может изменяться для соответствия настройкам существующей беспроводной сети или настройки сети для соответствующего клиента.<br>
			<strong><em>Ключи</em></strong> -
			Ключи 1-4 позволяют легко изменить настройки беспроводного шифрования для обеспечения безопасности беспроводной сети.
			Просто выберите определенный ключ, используемый для шифрования данных в сети.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>Персональный WPA</strong><br>
			Wi-Fi Protected Access позволяет осуществлять аутентификацию и авторизацию пользователей в беспроводной сети.
			Протокол WPA обеспечивает большую степень безопасности, чем WEP.
			Протокол WPA использует ключ, автоматически изменяемый через определенный интервал.
		</p>
		<p>
			<strong><em>Тип шифра</em></strong> -
			<?query("/sys/modelname");?> поддерживает два типа шифров при использовании WPA.
			Итак, доступны две опции: TKIP (Temporal Key Integrity Protocol) и AES (Advanced Encryption Standard).<br>
			<strong><em>PSK/EAP</em></strong> -
			При выборе опции PSK беспроводные клиенты должны будут ввести парольную фразу для аутентификации.
			При выборе опции EAP необходимо обеспечить наличие в сети RADIUS-сервера,
			который будет обрабатывать запросы аутентификации всех беспроводных клиентов.<br>
			<strong><em>Парольная фраза</em></strong> -
			Парольная фраза необходима беспроводным клиентам для взаимодействия с <?query("/sys/modelname");?>, когда выбрана опция PSK.
			Введите цифробуквенную строку длиной 8-63 символов. Запомните парольную фразу, поскольку ее нужно будет ввести на  всех беспроводных устройствах,
			которые необходимо добавить к беспроводной сети.<br>
			<strong><em>802.1X</em></strong> -
			При выборе данной опции аутентификация WPA используется в связке с RADIUS-сервером, который должен быть представлен в сети.
			Введите IP-адрес, порт и Shared Secret, настроенные на RADIUS-сервере. Также существует опция, позволяющая ввести информацию о втором RADIUS-сервере,
			когда в сети существует два RADIUS-сервера, используемые для аутентификации беспроводных клиентов.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>WPA2</strong><br>
			Wi-Fi Protected Access 2 позволяет авторизовать и аутентифицировать пользователей в беспроводной сети.
			WPA2 обеспечивает большую безопасность, по сравнению с WEP, и использует ключ, который меняется автоматически через заданный интервал.
		</p>
		<p>
			<strong><em>Тип шифра</em></strong> -
			<?query("/sys/modelname");?> поддерживает два типа шифра при использовании протокола WPA2.
			Итак, доступны две опции: TKIP (Temporal Key Integrity Protocol) и AES (Advanced Encryption Standard).<br>
			<strong><em>PSK/EAP</em></strong> -
			При выборе опции PSK беспроводные клиенты должны будут ввести парольную фразу для аутентификации.
			При выборе опции EAP необходимо обеспечить наличие в сети RADIUS-сервера, который будет обрабатывать запросы аутентификации всех беспроводных клиентов.<br>
			<strong><em>Парольная фраза</em></strong> -
			Парольная фраза необходима беспроводным клиентам для взаимодействия с <?query("/sys/modelname");?>, когда выбрана опция PSK.
			Введите цифробуквенную строку длиной 8-63 символов. Запомните парольную фразу, поскольку ее нужно будет ввести на  всех беспроводных устройствах,
			которые необходимо добавить к беспроводной сети.<br>
			<strong><em>802.1X</em></strong> -
			При выборе данной опции аутентификация WPA используется в связке с RADIUS-сервером, который должен быть представлен в сети.
			Введите IP-адрес, порт и Shared Secret, настроенные на RADIUS-сервере.
			Также существует опция, позволяющая ввести информацию о втором RADIUS-сервере, когда в сети существует два RADIUS-сервера,
			используемых для аутентификации беспроводных клиентов.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
 	<td height=20>
		<p>
			<strong>WPA2-Авто</strong><br>
			Выбор этой опции позволяет <?query("/sys/modelname");?> одновременно работать с клиентами, как WPA2, так и WPA.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<a name=03><strong>Настройка LAN</strong></a><br>
			Здесь представлены настройки интерфейса LAN (Local Area Network) устройства.
			Эти настройки можно рассматривать как &quot;приватные настройки&quot;.
			Существует возможность изменить IP-адрес LAN, если это необходимо.
			IP-адрес LAN относится к приватным настройкам и не виден из Интернет.
			По умолчанию установлен IP-адрес 192.168.0.1 и Маска подсети 255.255.255.0.<br><br>
			<strong><em>IP-адрес</em></strong> -
			IP-адрес <?query("/sys/modelname");?>, по умолчанию 192.168.0.1.<br>
			<strong><em>Маска подсети</em></strong> -
			Маска подсети <?query("/sys/modelname");?>, по умолчанию 255.255.255.0.<br>
			<strong><em>Локальное имя домена</em></strong> (дополнительно) - Введите локальное имя домена для сети.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong><em>DNS Relay</em></strong> -
			При включении опции DNS Relay DHCP-клиентам маршрутизатора будет назначен IP-адрес LAN маршрутизатора как адрес их DNS-сервера.
			Все запросы DNS, которые получит маршрутизатор, будут перенаправлены на DNS-сервер провайдера.
			Когда опция DNS relay выключена, всем DHCP-клиентам маршрутизатора будет назначен IP-адрес DNS-сервера провайдера.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=20>
		<p>
			<strong>DHCP-сервер</strong><br>
			DHCP расшифровывается как Протокол динамической конфигурации хоста (Dynamic Host Control Protocol).
			DHCP-сервер назначает IP-адреса устройствам в сети, запрашивающим их.
			В настройках этих устройств должно быть задано &quot;Получить IP-адрес автоматически&quot;.
			По умолчанию, DHCP-сервер включен в <?query("/sys/modelname");?>. Пул адресов DHCP-сервера содержит диапазон IP-адресов,
			которые будут автоматически назначаться устройствам в сети.
		</p>
		<p>
			<strong>DHCP Reservation</strong><br>
			Введите &quot;Имя компьютера&quot;, &quot;IP-адрес&quot; и &quot;MAC-адрес&quot; вручную для ПК,
			которому необходимо назначить статически IP-адрес или выберите ПК в выпадающем меню,
			которое содержит текущих DHCP-клиентов.
		</p>
		<p>
			<strong><em>Начальный IP-адрес</em></strong> Начальный IP-адрес для назначения DHCP-сервером.<br>
			<strong><em>Конечный IP-адрес</em></strong> Конечный IP-адрес диапазона адресов для назначения DHCP-сервером.<br>
			<strong><em>Время аренды</em></strong> Промежуток времени в минутах для аренды IP-адресов.
		</p>
		<p>
			Информация о подключенных компьютерах (динамических DHCP-клиентах) отображается в Таблице динамических DHCP-клиентов.
			В этой таблице показано Имя хоста, IP-адрес, MAC-адрес время истечения аренды DHCP для каждого компьютера.
		</p>
	</td>
</tr>
<tr><td height=20>&nbsp;</td></tr>
<tr><td height=20>&nbsp;</td></tr>
<tr>
	<td height=33>
		<p>
			<strong><a name=04>Время и Дата</a></strong>
		</p>
		<p>
			Время и Дата Настройки времени используются маршрутизатором для синхронизации сервисов, работающих по расписанию, и системного журнала.
			Необходимо выбрать часовой пояс, соответствующий географическому положению.
			Время может быть задано вручную или устройство может обратиться к NTP (Network Time Protocol)-серверу для получения настроек.
			Также можно установить даты перехода на сезонное время, и система автоматически изменит временные настройки в эту дату.
		</p>
		<p>
			<strong><em>Часовой пояс</em></strong> - Выберите часовой пояс, соответствующий географическому положению.<br>
			<strong><em>Переход на сезонное время</em></strong> -
			Если в часовом поясе соблюдается переход на сезонное время, включите эту опцию и определите Начальный месяц,
			Конечный месяц, Неделю, День и Время для перехода на сезонное время.<br>
			<strong><em>Автоматическая настройка времени</em></strong> -
			Выберите сервер времени, с которым <?query("/sys/modelname");?> будет синхронизировать время.
			Интервал, через который <?query("/sys/modelname");?>
			будет взаимодействовать с NTP-сервером D-Link установлен как 7 дней.<br>
			<strong><em>Установить время и дату вручную</em></strong> -
			Выберите эту опцию, чтобы задать настройки времени вручную. Необходимо задать Год,
			Месяц, День, Час, Минуты и Секунды, или же можно кликнуть по кнопке Копировать настройки времени с компьютера,
		</p>
	</td>
</tr>
<tr><td height=2>&nbsp;</td></tr>
<tr><td height=2>&nbsp;</td></tr>
<tr>
	<td height=44>
		<p>
			<strong><a name=05>Родительский контроль</a></strong>
		</p>
		<p>
			Наряду с возможностью фильтровать доступ определенных устройств к сети и Интернет,
			<?query("/sys/modelname");?> позволяет также разрешить или запретить доступ компьютерам в сети к определенным сайтам,
			заданным по URL-адресу или ключевому слову.
		</p>
		<p>
			<strong>Родительский контроль</strong><br>
			Функция Родительский контроль позволяет разрешить или запретить компьютерам в сети доступ к определенным
			Web-сайтам по ключевым словам или определенным Именам доменов. Выберите опцию &quot;ВКЛЮЧИТЬ Фильтрацию
			Web-сайтов и РАЗРЕШИТЬ компьютерам в сети доступ ТОЛЬКО к этим сайтам&quot;, чтобы разрешить компьютерам
			в сети доступ к определенным URL-адресам и Именам доменов. Выбор опции "ВКЛЮЧИТЬ Фильтрацию Web-сайтов и
			ЗАПРЕТИТЬ компьютерам в сети доступ ТОЛЬКО к этим сайтам&quot; позволяет запретить компьютерам в сети
			доступ к определенным URL-адресам и Именам доменов.
		</p>
		<p>
			<span class="style1">Пример 1:</span><br>
			Если необходимо заблокировать доступ пользователей LAN к любым Web-сайтам,
			URL-адрес которых содержит слово &quot;shopping&quot;,
			необходимо выбрать &quot;&quot;ВКЛЮЧИТЬ Фильтрацию Web-сайтов и ЗАПРЕТИТЬ компьютерам в сети доступ ТОЛЬКО к этим сайтам&quot;,
			а затем ввести слово &quot;shopping&quot; в список правил фильтрации Web-сайтов.
			Так, например, сайты, подобные приведенным ниже, будут заблокированы для пользователей LAN, поскольку они содержат ключевое слово в URL.<br>
			<a href="http://shopping.yahoo.com/">http://shopping.yahoo.com/</a><br>
			<a href="http://shopping.msn.com/">http://shopping.msn.com/</a>
		</p>
		<p>
			<span class="style1">Пример 2:</span><br>
			Если необходимо, чтобы дети получали доступ только к определенным сайтам, то выберите &quot;
			ВКЛЮЧИТЬ Фильтрацию Web-сайтов и РАЗРЕШИТЬ компьютерам в сети доступ ТОЛЬКО к этим сайтам&quot; и введите название нужных доменов.
		</p>
		<ul>
			<li>Google.com </li>
			<li>Cartoons.com </li>
			<li>Discovery.com </li>
		</ul>
	</td>
</tr>
</table>