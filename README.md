# Website sCrapper in C
---

## Example of a sconf file

```
=
{name -> My website} #Correspond to the name of your action
{url -> value} #Url of the website
+
{option1 -> value}
{option2 -> value}
{option3 -> value}

=
{name -> My second website} #Correspond to the name of your second action
{url -> value}		    #URL of the website
+
{max-depth -> 12}
{versionning -> on}

==
{name -> My great task}
{second -> 10} #Start 2 HTTP request every 10s
+
(My website, My second website)

== #Start the HTTP request every 2min and 10s
{name -> My easy task}
{minute -> 2}
{second -> 10}
+
(My website)

==
#Start the HTTP request every 6 hours
{name -> My easy task}
{hour -> 6}
+
(My second website)
```
