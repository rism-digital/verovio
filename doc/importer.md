Writing an importer
-------------------

Verovio supports import for Plain and Easy and for Darms codes (See the PaeInput and DarmsInput classes). Writing an importer from another code could follow the same approach. The code below illustrates how the following snippet can be created in Verovio.

![Snippet](https://cloud.githubusercontent.com/assets/3487289/9319255/2f5187de-4501-11e5-808b-897ed35aa6dd.png)

Some features (articulation, endings, etc.) are still missing but will be added eventually. The resulting MEI file is available [here](importer.mei) and the SVG output for version 0.9.9 [there](importer.svg).

``` C++
////////////////////////////////////
// The document
////////////////////////////////////

Doc *doc = new Doc();

////////////////////////////////////
// Some variables
////////////////////////////////////

Measure *measure = NULL;
Staff *staff = NULL;
Layer *layer = NULL;
Layer *layer2 = NULL;
Note *note = NULL;
Beam *beam = NULL;
Tuplet *tuplet = NULL;
Rest *rest = NULL;
MultiRest *multiRest = NULL;
Tie *tie = NULL;
Chord *chord = NULL;
Slur *slur = NULL;
ScoreDef *scoreDef = NULL;

// start and end id
std::string startid;
std::string endid;

////////////////////////////////////
// Score definition
////////////////////////////////////

// staffGrp and staffDef
StaffGrp *staffGrp = new StaffGrp();
StaffDef *staffDef = new StaffDef();
staffDef->SetN( 1 );
staffDef->SetLines(5);

// clef
staffDef->SetClefLine(2);
staffDef->SetClefShape(CLEFSHAPE_G);

// key and meter signatures can be put at in the doc scoreDef
m_doc->m_scoreDef.SetKeySig(KEYSIGNATURE_2s);
m_doc->m_scoreDef.SetMeterSym(METERSIGN_common);

// adding the staffDef to the staffGrp and the staffGrp to the doc scoreDef
staffGrp->AddStaffDef(staffDef);
m_doc->m_scoreDef.AddStaffGrp(staffGrp);

// we need one page and one system
Page *page = new Page();
m_doc->AddPage(page);
System *system = new System();
page->AddSystem(system);

////////////////////////////////////
// Up-beat measure
////////////////////////////////////

// create the measure and its content
measure = new Measure();
measure->SetN(0);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
rest = new Rest();
rest->SetDur(DURATION_8);

// add them up
layer->AddLayerElement(rest);
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);

////////////////////////////////////
// Multi-rest measure
////////////////////////////////////

// create another measure and content
measure = new Measure();
measure->SetN(1);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);

// add a multi-rest
multiRest = new MultiRest();
multiRest->SetNum(3);

// add them up
layer->AddLayerElement(multiRest);
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);

////////////////////////////////////
// Fourth measure
////////////////////////////////////

measure = new Measure();
measure->SetN(4);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);

tuplet = new Tuplet;
tuplet->SetNum(3);
tuplet->SetNumbase(2);
beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(5);
beam->AddLayerElement(note);
tuplet->AddLayerElement(beam);
layer->AddLayerElement(tuplet);

tuplet = new Tuplet;
tuplet->SetNum(3);
tuplet->SetNumbase(2);
beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetDots(1);
note->SetPname(PITCHNAME_f);
note->SetOct(5);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_16);
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_n);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);

// keep the id as start id for the tie
startid = note->GetUuid();
beam->AddLayerElement(note);
tuplet->AddLayerElement(beam);
layer->AddLayerElement(tuplet);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_g);
note->SetOct(5);

// keep the id as end id for the tie
endid = note->GetUuid();
layer->AddLayerElement(note);

// create the tie with appropriate start and end ids
tie = new Tie();
tie->SetStartid(startid);
tie->SetEndid(endid);

beam = new Beam();
chord = new Chord();
chord->SetDur(DURATION_8);
note = new Note();
note->SetPname(PITCHNAME_a);
note->SetOct(5);
startid = note->GetUuid();
chord->AddLayerElement(note);
note = new Note();
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
chord->AddLayerElement(note);
beam->AddLayerElement(chord);
chord = new Chord();
chord->SetDur(DURATION_8);
note = new Note();
note->SetPname(PITCHNAME_g);
note->SetOct(5);
chord->AddLayerElement(note);
note = new Note();
note->SetPname(PITCHNAME_b);
note->SetOct(4);
chord->AddLayerElement(note);
beam->AddLayerElement(chord);
layer->AddLayerElement(beam);

// add them up
staff->AddLayer(layer);
measure->AddStaff(staff);

// add the tie element to the measure
measure->AddFloatingElement(tie);
system->AddMeasure(measure);

////////////////////////////////////
// Fifth measure
////////////////////////////////////

measure = new Measure();
measure->SetN(5);
measure->SetRight(BARRENDITION_rptstart);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);

note = new Note();
note->SetGrace(GRACE_acc);
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(5);
note->SetStemDir(STEMDIRECTION_up);
layer->AddLayerElement(note);

chord = new Chord();
chord->SetDur(DURATION_breve);
note = new Note();
note->SetPname(PITCHNAME_g);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_n);
endid = note->GetUuid();
chord->AddLayerElement(note);
note = new Note();
note->SetPname(PITCHNAME_d);
note->SetOct(5);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
chord->AddLayerElement(note);
layer->AddLayerElement(chord);

// create the slur
slur = new Slur();
slur->SetStartid(startid);
slur->SetEndid(endid);

// add the slur to the previous measure
dynamic_cast<Measure*>(system->m_children.back())->AddFloatingElement(slur);

// add the content
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);

////////////////////////////////////
// Changing the scoreDef
////////////////////////////////////

scoreDef = new ScoreDef();
scoreDef->SetMeterCount(6);
scoreDef->SetMeterUnit(8);
scoreDef->SetKeySig(KEYSIGNATURE_0);

system->AddScoreDef(scoreDef);

////////////////////////////////////
// Sixth measure
////////////////////////////////////

measure = new Measure();
measure->SetN(6);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
layer2 = new Layer();
layer2->SetN(2);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_e);
note->SetOct(5);
layer->AddLayerElement(note);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_d);
note->SetOct(5);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddLayerElement(note);
layer->AddLayerElement(beam);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(4);
beam->AddLayerElement(note);
layer2->AddLayerElement(beam);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_f);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(4);
layer2->AddLayerElement(note);

// add them up
staff->AddLayer(layer);
staff->AddLayer(layer2);
measure->AddStaff(staff);
system->AddMeasure(measure);

////////////////////////////////////
// Seventh measure
////////////////////////////////////

measure = new Measure();
measure->SetN(7);
measure->SetRight(BARRENDITION_rptend);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);
layer2 = new Layer();
layer2->SetN(2);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
layer->AddLayerElement(note);

note = new Note();
note->SetDur(DURATION_4);
note->SetDots(1);
note->SetPname(PITCHNAME_d);
note->SetOct(5);
layer->AddLayerElement(note);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_e);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_g);
note->SetOct(4);
beam->AddLayerElement(note);
layer2->AddLayerElement(beam);

beam = new Beam();
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_f);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_b);
note->SetOct(4);
beam->AddLayerElement(note);
note = new Note();
note->SetDur(DURATION_8);
note->SetPname(PITCHNAME_a);
note->SetAccid(ACCIDENTAL_EXPLICIT_s);
note->SetOct(4);
beam->AddLayerElement(note);
layer2->AddLayerElement(beam);

// add them up
staff->AddLayer(layer);
staff->AddLayer(layer2);
measure->AddStaff(staff);
system->AddMeasure(measure);

////////////////////////////////////
// Eighth measure
////////////////////////////////////

measure = new Measure();
measure->SetN(8);
measure->SetRight(BARRENDITION_end);
staff = new Staff();
staff->SetN(1);
layer = new Layer();
layer->SetN(1);

chord = new Chord();
chord->SetDur(DURATION_4);
note = new Note();
note->SetPname(PITCHNAME_c);
note->SetAccidGes(ACCIDENTAL_IMPLICIT_s);
note->SetOct(5);
chord->AddLayerElement(note);
note = new Note();
note->SetPname(PITCHNAME_e);
note->SetOct(4);
note->SetFermata(PLACE_below);
chord->AddLayerElement(note);
layer->AddLayerElement(chord);

// add them up
staff->AddLayer(layer);
measure->AddStaff(staff);
system->AddMeasure(measure);
```


