
### Ongoing repo


![drawing01](/drawings/HEMY_Drawing_00001.png)
![drawing02](/drawings/HEMY_Drawing_00002.png)
![drawing03](/drawings/HEMY_Drawing_00003.png)
![drawing04](/drawings/HEMY_Drawing_00004.png)
![drawing05](/drawings/HEMY_Drawing_00005.png)
![drawing06](/drawings/HEMY_Drawing_00006.png)
![drawing07](/drawings/HEMY_Drawing_00007.png)
![drawing08](/drawings/HEMY_Drawing_00008.png)
![drawing09](/drawings/HEMY_Drawing_00009.png)
![drawing10](/drawings/HEMY_Drawing_00010.png)
![drawing11](/drawings/HEMY_Drawing_00011.png)
![drawing12](/drawings/HEMY_Drawing_00012.png)
![drawing13](/drawings/HEMY_Drawing_00013.png)
![drawing14](/drawings/HEMY_Drawing_00014.png)
![drawing15](/drawings/HEMY_Drawing_00015.png)
![drawing16](/drawings/HEMY_Drawing_00016.png)



# HEM*Y*: Harmonic Equatorial Mount *Yourself*
## What the hell is this HEM*Y* thing?

HEMY (Harmonic Equatorial Mount *Yourself*) is an open-source harmonic equatorial mount for observation or astrophotography. It is based on the use of a harmonic reducer. The mount is designed to be inexpensive in relation to its performance, transportable, with a relatively large load capacity. Most importantly, this mount is easy to build: no need for machine tools or other laser-cutting-water-jet-plasma-turbo-piston-5-axis machines, you can order the CNC aluminium parts for less than 300€. A shopping list, a screwdriver, a saw and some resourcefulness: you've got everything you need to make an HEM*Y*. The ultimate aim is to distribute its design as an open-source product for personal use.

The technology used in this mount is similar to that used in robotics or industrial production lines. The main mechanical gearbox is a harmonic gearbox ([wikipedia](https://en.wikipedia.org/wiki/Strain_wave_gearing)). 

<img align="right" src="https://upload.wikimedia.org/wikipedia/commons/thumb/2/21/HarmonicDriveAni.gif/240px-HarmonicDriveAni.gif" width="15%">

It can produce surprising results for an astronomical mount at first glance; the mount appears grossly undersized and unbalanced, whereas the whole assembly is extremely robust and stable, with enormous torque.
Conventional equatorial mounts require the use of counterweights. The aim of this development is to make available the design of a harmonic mount that can be built in your garage, with few tools or mechanical knowledge, for a price of less than 800€. Of course, the proud aim is to achieve performance equivalent to that of commercial harmonic reduction mounts.


The first results of the performance tests are up to my expectations for the moment. I hope this github repository can help you design or reproduce this equatorial mount. Please be patient, I'm only working on this project in my spare time. I'll update the testing progress on this readme. ✍️

## Support

Hey! Help me out for a couple of :beers: !

<a href="https://www.buymeacoffee.com/polvinc" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-green.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>

## What are the specifications


# Assembly

## Please read PDF drawings
## Please watch STEP files
## BOM
| Name | Quantity | Reference | Price € | Link |
| -------- | -------- | -------- | -------- | -------- |
20mm Bosch Rexroth profile | 1 | 212-3292 | 25 | [RS components](https://fr.rs-online.com/web/p/tubes-et-profiles-de-structures/2123292?gb=s)
T-nuts | 64 | 466-7281 | 70 | [RS components](https://fr.rs-online.com/web/p/raccords-pour-elements-de-structure/4667281)
Dovetail Clamp | 1 | EUF9144C | 30 | [Amazon FR](https://www.amazon.fr/gp/product/B08TC111QC/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
Dovetail Mounting Plate | 1 | EUF9175B | 20 | [Amazon FR](https://www.amazon.fr/gp/product/B08XLWP13X/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
| Nema 17 0.9° double shaft| 2 | 17HS15-1584S-MG10 | 15 | [StepperOnline](https://www.omc-stepperonline.com/fr/biaxial-nema-17-bipolaire-0-9deg-44ncm-62-48oz-in-1-68a-2-8v-42x42x48mm-4-fils-17hm19-1684d)
Harmonic Drive reduction 1:100| 2 | HBS-17 | 550 | [Aliexpress](https://fr.aliexpress.com/item/1005007101363756.html?gatewayAdapt=glo2fra)
RJ45 connector | 2 | MRJ5380M1 | 35 | [Farnell](https://fr.farnell.com/amphenol-communications-solutions/mrj5380m1/conn-rj45-90-jack-8p8c-1-port/dp/4122450)
CNC parts | 18 | - | 330 HT | [JLC3DP](https://jlcpcb.com/)


## Harmonic drive
Harmonic gearboxes ([wikipedia](https://en.wikipedia.org/wiki/Strain_wave_gearing)), or more precisely strain wave gears, are a type of gear mechanism that transmits much more torque than other common gears. This is mainly due to the fact that there are many more teeth engaged at once. One of the major advantages of these gears is that they have virtually no backlash, partly because they use deformation. It's a bit like belts, which are supposed to be more or less backlash-free, but harmonic gearboxes use deformable metal spring steels, which are much better than belts.

Put another way, in the case of an astronomical mount: they can drive much heavier loads without the need for counterweights, all with a very low self-weight!

The harmonic reducer model chosen [CSF-17-100-2UH](https://harmonicdrive.de/fileadmin/user_upload/Harmonic_Drive_Gears_EN_1050860_06_2022.pdf#page=50) is the same as [Alan](https://alanz.info/posts/2022/07/diy-mount/) DIY mount. You can find lots of them on ebay. This one has a reduction ratio of 1/100. It has the good fortune to meet my requirements, and to have been tested for another equatorial mount. Why deprive yourself? According to datasheet, it can handle more than 50Nm of torque and it also has a very comfortable permissible radial load of over 70Kg.
On AliExpress, you can find [HBS-17](https://fr.aliexpress.com/item/1005007101363756.html?gatewayAdapt=glo2fra), much cheaper.

<p align="center">
  <img src="images/OnStep_calc.png" width="96.8%">
</p>

Using [OnStep's configurator spreadsheet](http://o.baheyeldin.com:1111), I chose to drive the harmonic gearbox with a belt/pulley couple, in order to achieve good tracking resolution, a good slew rate and a good number of steps/deg. The reduction ratio is 1/5 on RA, and 1/3 on DEC.
