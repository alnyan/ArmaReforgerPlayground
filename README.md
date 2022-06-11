alnyan's Arma Reforger Playground
=================================

This is an experiments playground repository which may some day become a real addon.

Currently it contains (sorry for very work-in-progress quality):

* BM-21 launcher turret, with ability to input heading/elevation data and fire rockets
* Ammo for BM-21 -- 9M28F HE rocket
* Modded Ural 4320 platform with BM-21 turret attached


Can be useful for referencing in your addons:

* Blender files for all the models with proc animation support
* Simple launcher pod controller component, which takes heading/elevation data and uses proc animation to turn the turret
* Simple rocket launch UserAction, which looks for a rocket in SlotManagerComponent and activates it
* Heading/elevation input UI for the turret


`TODO: Make some guides on how I actually made this stuff`
