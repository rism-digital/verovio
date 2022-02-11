import argparse
import json
import os
import sys

import diffimg
import lxml.etree as etree
import PIL.Image as Image
import PIL.ImageChops as ImageChops
import PIL.ImageOps as ImageOps
import xmldiff.main as main

ns = {'svg': 'http://www.w3.org/2000/svg'}

linkToTestSuiteCat = 'https://www.verovio.org/test-suite.xhtml?cat='
linkToTestInEditor = 'https://editor.verovio.org/?file=https://raw.githubusercontent.com/rism-digital/verovio.org/gh-pages/_tests/'


def start_webpage():
    html = etree.Element('html')
    head = etree.Element('head')
    html.append(head)
    title = etree.SubElement(head, 'title')
    title.text = 'Verovio test suite for Pull Request'

    etree.SubElement(head, 'link', attrib={
                     'rel': 'stylesheet', 'href': 'https://www.verovio.org/css/bootstrap/css/bootstrap.min.css', 'type': 'text/css'})
    etree.SubElement(head, 'link', attrib={
                     'rel': 'stylesheet', 'href': 'https://www.verovio.org/css/bootstrap/css/bootstrap-theme.min.css', 'type': 'text/css'})
    etree.SubElement(head, 'link', attrib={
                     'rel': 'stylesheet', 'href': 'https://www.verovio.org/css/verovio.css', 'type': ' text/css'})

    etree.SubElement(head, 'script', attrib={
                     'src': 'https://www.verovio.org/javascript/jquery.min.js', 'type': 'text/javascript'})
    etree.SubElement(head, 'script', attrib={
                     'src': 'https://www.verovio.org/javascript/bootstrap.min.js', 'type': 'text/javascript'})

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
        $("a.before-after").click(function(e){
            e.preventDefault();
            $(this).parent().next('.img-before-after').toggle();
        });
    });
    """
    script = etree.SubElement(body, 'script', attrib={
                              'type': 'text/javascript'})
    script.text = js

    docType = '<!DOCTYPE html>'
    tree = etree.ElementTree(html)
    tree.write(htmlFileOut, xml_declaration=False,
               doctype=docType, method='html', pretty_print=True)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='Examine diffrences in renderings of MEI scores')
    parser.add_argument('input_dir1')
    parser.add_argument('input_dir2')
    parser.add_argument('output_dir')
    parser.add_argument('-t', '--threshold', required=False, type=float,
                        default=0.1, help='Threshold for visual difference in percent')
    args = parser.parse_args()

    html, body = start_webpage()
    div = etree.Element('div', attrib={'class': 'container'})
    body.append(div)
    text1 = etree.SubElement(div, 'h3')
    text2 = etree.SubElement(div, 'p')
    ulTablist = etree.SubElement(
        div, 'ul', attrib={'class': 'nav nav-pills', 'role': 'tablist'})
    divTabcontent = etree.SubElement(
        div, 'div', attrib={'class': 'tab-content'})

    totalChanges = 0
    categoryChanges = 0
    log = []

    path_in1 = args.input_dir1
    path_in2 = args.input_dir2
    path_out = args.output_dir
    dir1 = sorted(os.listdir(path_in1))
    for item1 in dir1:
        if not(os.path.isdir(os.path.join(path_in1, item1))):
            continue

        # create the output directory if necessary
        if not(os.path.isdir(os.path.join(path_out, item1))):
            os.mkdir(os.path.join(path_out, item1))

        divTab = etree.SubElement(divTabcontent, 'div', attrib={
                                  'id': item1, 'class': 'tab-pane', 'role': 'tabpanel'})
        etree.SubElement(divTab, 'hr')
        h4 = etree.SubElement(divTab, 'h4')
        a = etree.SubElement(
            h4, 'a', attrib={'href': linkToTestSuiteCat + item1, 'target': '_blank'})
        a.text = 'Open the test suite for this category'
        table = etree.SubElement(divTab, 'table', attrib={
                                 'class': 'table table-bordered'})

        nbChanges = 0

        dir2 = sorted(os.listdir(os.path.join(path_in1, item1)))
        for item2 in dir2:
            # skip directories
            if not(os.path.isfile(os.path.join(path_in1, item1, item2))):
                continue
            # skip hidden files and not SVG files
            if item2.startswith('.') or not(item2.endswith('.svg')):
                continue

            svgFile1 = os.path.join(path_in1, item1, item2)
            svgFile2 = os.path.join(path_in2, item1, item2)

            # filenames (input SVG and output PNG)
            name, ext = os.path.splitext(item2)
            pngFile1 = os.path.join(path_in1, item1, name + '.png')
            pngFile2 = os.path.join(path_in2, item1, name + '.png')
            pngFileOut = os.path.join(path_out, item1, name + '.png')
            pngFile1Out = os.path.join(path_out, item1, name + '.after.png')
            pngFile2Out = os.path.join(path_out, item1, name + '.before.png')
            print(f'Comparing {name}')

            diffValue = diffimg.diff(pngFile1, pngFile2, delete_diff_file=True)
            if (diffValue > (args.threshold / 100.0)):
                print(f'Img diff: {diffValue}')
                row = etree.SubElement(table, 'tr')
                col = etree.SubElement(row, 'td')
                p = etree.SubElement(col, 'p')
                p.text = name + ' (diff:' + str(diffValue) + ') - '
                # link to Verovio editor
                a = etree.SubElement(p, 'a', attrib={
                                     'href': linkToTestInEditor + item1 + '/' + name + '.mei', 'target': "_blank"})
                a.text = 'Open this test in the Verovio Editor'
                etree.SubElement(col, 'img', attrib={
                                 'src': item1 + '/' + name + '.png', 'class': 'img-responsive'})
                # link to show before - after
                p = etree.SubElement(col, 'p')
                a = etree.SubElement(
                    p, 'a', attrib={'href': '#', 'class': 'before-after'})
                a.text = 'Show before / after'
                divBeforeAfter = etree.SubElement(
                    col, 'div', attrib={'class': 'img-before-after', 'style': 'display: none'})
                etree.SubElement(divBeforeAfter, 'img', attrib={
                                 'src': item1 + '/' + name + '.before.png', 'class': 'img-responsive'})
                etree.SubElement(divBeforeAfter, 'img', attrib={
                                 'src': item1 + '/' + name + '.after.png', 'class': 'img-responsive'})

                nbChanges += 1

                im1 = Image.open(pngFile1)
                im1.save(pngFile1Out)
                im2 = Image.open(pngFile2)
                im2.save(pngFile2Out)
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

            for e in root1.findall('./svg:desc', namespaces=ns):
                root1.remove(e)

            # remove //svg/desc (version will be different)
            for node in root2.xpath('//*[@*]'):
                node.attrib.clear()

            for e in root2.findall('./svg:desc', namespaces=ns):
                root2.remove(e)

            # unused for now
            diff = main.diff_trees(root1, root2)
            if (len(diff) > 0):
                print(f'Node diff: {len(diff)}')

        if (nbChanges > 0):
            li = etree.SubElement(ulTablist, 'li', attrib={
                                  'role': 'presentation'})
            link = etree.SubElement(li, 'a', attrib={
                                    'data-toggle': 'tab', 'role': 'tab', 'aria-controls': item1, 'href': '#' + item1})
            link.text = item1.capitalize() + ' '
            span = etree.SubElement(link, 'span', attrib={'class': 'badge'})
            span.text = str(nbChanges)

            log.append("****** {}: {} ******".format(item1, nbChanges))
            categoryChanges += 1
            totalChanges += nbChanges

    text1.text = 'Test suite visual regression test for PR'
    text2.text = '{} change(s) detected in {} test category(ies)'.format(
        totalChanges, categoryChanges)

    htmlFileOut = os.path.join(path_out, 'index.html')
    end_webpage(html, body, htmlFileOut)

    if (totalChanges > 0):
        logFileOut = os.path.join(path_out, 'log.md')
        with open(logFileOut, 'w') as f:
            f.write("\n%s\n" % text2.text)
            for item in log:
                f.write("%s\n" % item)
