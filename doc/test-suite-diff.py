import argparse
import diffimg
import json
import lxml.etree as etree
import PIL.Image as Image
import PIL.ImageChops as ImageChops
import PIL.ImageOps as ImageOps
import os
import sys
import xmldiff.main as main

ns = {'svg': 'http://www.w3.org/2000/svg'}

def start_webpage():
    html = etree.Element('html')
    head = etree.Element('head')
    html.append(head)
    title = etree.SubElement(head, 'title')
    title.text = "Verovio test suite for Pull Request"

    etree.SubElement(head, 'link', attrib={'rel': 'stylesheet', 'href': 'https://www.verovio.org/css/bootstrap/css/bootstrap.min.css', 'type': 'text/css' })
    etree.SubElement(head, 'link', attrib={'rel': 'stylesheet', 'href': 'https://www.verovio.org/css/bootstrap/css/bootstrap-theme.min.css', 'type': 'text/css' })
    etree.SubElement(head, 'link', attrib={'rel': 'stylesheet', 'href': 'https://www.verovio.org/css/verovio.css', 'type':' text/css' })

    etree.SubElement(head, 'script', attrib={'src': 'https://www.verovio.org/javascript/jquery.min.js', 'type': 'text/javascript' })
    etree.SubElement(head, 'script', attrib={'src': 'https://www.verovio.org/javascript/bootstrap.min.js', 'type': 'text/javascript' })

    body = etree.Element('body')

    html.append(body)
    return html, body

def end_webpage(html, body, htmlOutFile):
    js = """
    $(document).ready(function(){ 
        $(".nav-tabs a").click(function(e){
            e.preventDefault();
            $(this).tab('show');
        });
    });
    """
    script = etree.SubElement(body, 'script', attrib={'type': 'text/javascript'})
    script.text = js
    
    docType = '<!DOCTYPE html>'
    tree = etree.ElementTree(html)
    tree.write(htmlFileOut, xml_declaration=False, doctype=docType, method='html', pretty_print=True)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("input_dir1")
    parser.add_argument("input_dir2")
    parser.add_argument("output_dir")
    args = parser.parse_args()

    html, body = start_webpage()
    div = etree.Element('div', attrib={'class': 'container'})
    body.append(div)
    ulTablist = etree.SubElement(div, 'ul', attrib={'class': 'nav nav-tabs', 'role': 'tablist'})
    divTabcontent = etree.SubElement(div, 'div', attrib={'class': 'tab-content'})

    path_in1 = args.input_dir1
    path_in2 = args.input_dir2
    path_out = args.output_dir
    for item1 in os.listdir(path_in1):
        if not(os.path.isdir(os.path.join(path_in1, item1))): continue

        # create the output directory if necessary
        if not(os.path.isdir(os.path.join(path_out, item1))):
            os.mkdir(os.path.join(path_out, item1))

        divTab = etree.SubElement(divTabcontent, 'div', attrib={'id': item1, 'class': 'tab-pane', 'role': 'tabpanel'})
        table = etree.SubElement(divTab, 'table', attrib={'class': 'table table-bordered'})

        nbChanges = 0
        
        for item2 in os.listdir(os.path.join(path_in1, item1)):
            # skip directories
            if not(os.path.isfile(os.path.join(path_in1, item1, item2))): continue
            # skip hidden files and not SVG files
            if item2.startswith('.') or not(item2.endswith('.svg')): continue

            svgFile1 = os.path.join(path_in1, item1, item2)
            svgFile2 = os.path.join(path_in2, item1, item2)
            
            # filenames (input SVG and output PNG)
            name, ext = os.path.splitext(item2)
            pngFile1 = os.path.join(path_in1, item1, name + '.png')
            pngFile2 = os.path.join(path_in2, item1, name + '.png')
            pngFileOut = os.path.join(path_out, item1, name + '.png')
            print(pngFile1, pngFile2)

            diffValue = diffimg.diff(pngFile1, pngFile2, delete_diff_file=True)
            if (diffValue > 0.0):
                print("Img diff:", diffValue)
                row = etree.SubElement(table, 'tr')
                col = etree.SubElement(row, 'td')
                p = etree.SubElement(col, 'p')
                p.text = name + ' (diff:' + str(diffValue) + ')'
                img = etree.SubElement(col, 'img', attrib={'src': item1 + '/' + name + '.png', 'class': 'img-responsive'})
                nbChanges += 1

                im1 = Image.open(pngFile1)
                im2 = Image.open(pngFile2)
                difference = ImageChops.difference(im1, im2)

                diffIm1 = ImageOps.fit(im2, difference.size)
                R, G, B, A = difference.split()
                mask = Image.new('L', diffIm1.size, 255)
                diffIm2 = Image.merge('RGBA', (mask, G, B, A)) 
                diffIm2.paste(diffIm1, (0, 0), diffIm1)
                diffIm2.save(pngFileOut)  

            tree1 = etree.parse(svgFile1)
            root1 = tree1.getroot()
                       
            tree2 = etree.parse(svgFile2)
            root2 = tree2.getroot()

            # remove all attributes
            for node in root1.xpath('//*[@*]'):
                node.attrib.clear()

            for e in root1.findall("./svg:desc", namespaces=ns):
                root1.remove(e)
            
            # remove //svg/desc (version will be different)
            for node in root2.xpath('//*[@*]'):
                node.attrib.clear()

            for e in root2.findall("./svg:desc", namespaces=ns):
                root2.remove(e)

            # unused for now
            diff = main.diff_trees(root1, root2)
            if (len(diff) > 0):
                print("Node diff:", len(diff))

        if (nbChanges > 0):
            li = etree.SubElement(ulTablist, 'li', attrib={'role': 'presentation'})
            link = etree.SubElement(li, 'a', attrib={'data-toggle': 'tab', 'role': 'tab', 'aria-controls': item1, 'href': '#' + item1})
            link.text = item1.capitalize() + ' '
            span = etree.SubElement(link, 'span', attrib={'class': 'badge'})
            span.text = str(nbChanges)
    
    htmlFileOut = os.path.join(path_out, 'index.html')
    end_webpage(html, body, htmlFileOut)
