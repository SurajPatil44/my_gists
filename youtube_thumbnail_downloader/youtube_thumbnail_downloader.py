import requests
import re
import os

SMALLEST_FILE_SIZE = 2 * 1024
            
def get_query_result(query):
    base_url = "https://www.youtube.com"
    s_url = f"{base_url}/results?search_query={query}"
    pattern = r"watch\?v=(\S{11})"
    r = requests.get(s_url)
    return re.findall(pattern,r.text)

def save_from_id(id,pathname,url,second):
    raw = requests.get(url)
    fname = f"{id}.jpg"
    fname = os.path.join(pathname,fname)
    print(f"Saving file {fname}......")
    with open(fname,"wb+") as f:
        img = raw.content
        sz = len(img)
        if sz > SMALLEST_FILE_SIZE:
            f.write(img)
        else:
            u2 = f"http://img.youtube.com/vi/{id}/hqdefault.jpg"
            if not second:
                save_from_id(id,pathname,u2,second=True)
            else:
                print(f"can't save {url}")
    print(f"file {fname} saved......")
        
def save_thumbnails(results,base='.',folder="thumbnails"):
    #folder = "thumbnails"
    ## Create folder at current location
    if base == '.':
        base = os.getcwd()
    fpath = os.path.join(base,folder)
    if not os.path.exists(fpath):
        print(f"creating {fpath}....")
        os.mkdir(fpath)
    for res in results:
        url = f"http://img.youtube.com/vi/{res}/maxresdefault.jpg"
        save_from_id(res,fpath,url,False)
        
def run(query,base=".",folder="thumbnails"):
    results = get_query_result(query)
    print(results)
    save_thumbnails(results,base,folder)

if __name__ == "__main__":
    run("macbook pro m1")