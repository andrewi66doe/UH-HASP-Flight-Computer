import bs4 as soup
import urllib.request as r

url = 'http://laspace.lsu.edu/hasp/groups/2017/data/data.php?pname=Payload_10&py=2017'

# connect to url and read the html as soup
req  = r.Request("http://laspace.lsu.edu/hasp/groups/2017/data/data.php?pname=Payload_10&py=2017")
with r.urlopen(req) as response: 
    markup = response.read()

# parse the html to find the text from cells and retrieve the download links for each file
# tag is here to concatenate to the links povided from the retrival 
html = soup.BeautifulSoup(markup ,"html.parser")
cells = html.find_all('td')
table = html.find('table','flightdata')
downloadlinks = table.find_all('a') 
tag = "http://laspace.lsu.edu/hasp/groups/2017/data/"

# empty lists that will hold the data per respected field
content = []
filenames = []
timestamps = []
filesizes = []
links = []

# fill contents list with the text from each cell 
for td in cells:
    content.append(td.text)

# iterate through content to retrive individual fields 
# content: filename1, timestamp1, filesize1, filename2, timestamp2, filesize2...
for e in range(0, len(content), 3):
    filenames.append(content[e])
for e in range(1, len(content), 3):
    timestamps.append(content[e])
for e in range(2, len(content), 3):
    filesizes.append(content[e])

# downloadlinks contains <a></a> tags 
# iterate through downloadlinks and extract the href link then concatenate with tag
# then append that to links
for e in downloadlinks:
    links.append(tag+e.get('href'))

# uncomment to download all contents in the table 
#for e in range(0, len(links)):
#    r.urlretrieve(links[e], filenames[e])

# open each file downloaded then append its contents to data.txt
for filename in filenames:
    with open(filename, 'r', encoding='latin-1') as infile:
        data = infile.read()
    with open("data.txt", 'a') as outfile:
        outfile.write(data)
