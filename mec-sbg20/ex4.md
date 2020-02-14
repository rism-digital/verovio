---
layout: iaml19
title: Music Engraving Conference - Verovio workshop
active: none
---

### Music Engraving Conference – Exercise 4 – MEI editing with Verovio

In this exercise, you can try to do some MEI encoding using the Verovio online editor. It allows you to enter MEI code and to have it rendered in realtime.

Start with [this](https://www.verovio.org/editor/?file=/examples/hello-world/single-note.mei){:target="_blank"} and try to encode the first measure of the following example:

![image](https://user-images.githubusercontent.com/689412/57692051-4e450080-7635-11e9-9e9b-0dfa44d88803.png){:width="500px"}


#### First staff

1. Add a system group with `staffGrp@symbol="bracket"`

2. Add the instrument with:

```xml
<staffDef>
  <label>Oboe I, II</label>
</staffDef>
```

3. Add the meter signature with `scoreDef@meter.count="3"` and `scoreDef@meter.unit="4"`

4. Add a tempo indication, in `<measure>`:
	
```xml
<tempo>Adagio</tempo>
```

5. Set the position of the tempo with `tempo@tstamp="1"` und `staff="1"` (not needed with only on staff)

See [time stamps in guidelines](https://music-encoding.org/guidelines/v4/content/cmn.html#cmnTstamp){:target="_blank"}

6. Add a forte with `<dynam>`

See [dynam in guidelines](https://music-encoding.org/guidelines/v4/content/shared.html#sharedExpressionMarks){:target="_blank"}

7. Add a bean an notes:

```xml
<beam>
  <note/>
  <rest/>
  <note/>
</beam>
```

8. Add a second voice with `<layer>` and `layer@n="2"`.

#### Second staff

1. Add a staff (definition) with `<staffDef>` and `staffDef@n="2"`.

2. Add a staff (content) with `<staff>` and `staff@n="2"`.

3. Add a "a 2" directive with `<dir>`

4. Make the text italic with `<rend>` and `rend@?`

See [Text rendition in guidelines](https://music-encoding.org/guidelines/v4/content/lyricsperfdir.html#sharedTextRendition){:target="_blank"}

5. Apply the forte to both staves with `dynam@staff="1 2"`

Is this representative of the content?

See [Guidelines](https://music-encoding.org/guidelines/v4/content/analysisharm.html#analysisDescribingRelationships){:target="_blank"}

What about the third staff?

