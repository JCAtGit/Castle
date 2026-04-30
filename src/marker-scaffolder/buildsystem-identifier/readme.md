#Aanpak
	Identificeer standaard procedures voor het compileren
	Vertaal procedures om naar bepaalde architecturen, sommige ondersteunen alleen een specifieke arch (e.g. rufus.)
		Alternative, indien nodig:
			leg voor repos met specifieke eisen, custom compile formats vast die de repo dan bouwt, neem hierbij details op van repo zodat build voorspelbaar 			
			blijft.
	
	Identificeer compileer regels in project
	Vervang compileer regels met een include of inline van eigen compileer opties
	Bouw eerst met aangepaste opties
	Bouw daarna met gemarkte bestanden
