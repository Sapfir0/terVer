function parsing(object) {
    if (object == undefined) {
        return "";
    }
    var res = "";
    res += `virtualElement(virtualElement::${object.type})`
    for (item in object.values) {
        res += `.addElement(${
            typeof object.values[item] == "string" ? `realElement(${object.values[item]}, "${object.values[item]}")` : `${parsing(object.values[item])}`
        })`
    }

    return res;
}

function main() {
    fetch('./formul.json').then(response => {
        if (response.ok) {
            return response.json();
        }
        else {
            throw new error("саня, проснись, ты обосрался.");
        }
    } ).then(json => {
        const res = parsing(json);
        const text_elem = document.querySelector("div.formul_text")
        console.log(text_elem);
        text_elem.innerHTML = "";
        text_elem.insertAdjacentText("beforeend", res);

    }).catch(error => console.error(error))
}

addEventListener("DOMContentLoaded", main);