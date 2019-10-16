#!/usr/bin/env python
# coding: utf-8

# In[31]:


import pandas as pd


# In[32]:


output = pd.read_csv('analysis.csv')


# In[33]:


IP = '10.21.2.75'


# In[34]:


print("Average Packet Size: {} bytes".format(sum(output['Length'])/len(output['Length'])))


# In[35]:


print('Average no of Packets sent per flow: {}'.format(output[output['Source']==IP].groupby(['Source','Destination'])['Length'].count().mean()))


# In[36]:


print('Average no of Bytes sent per flow: {}'.format(output[output['Source']==IP].groupby(['Source','Destination'])['Length'].sum().mean()))


# In[37]:


x = len(output[output['Destination']==IP]['Length'])/len(output[output['Source']==IP]['Length'])
print('Average Ratio of incoming to outgoing packets: {}'.format(x))


# In[38]:


x = (output[output['Source']==IP]['Time'])
ans = (x.iloc[len(x)-1] - x.iloc[0])/len(x)
print('Average Time interval between packets sent: {}'.format(ans))


# In[39]:


n_destinations = len(set(output[output['Source']==IP]['Destination']))
print('Average connections to number of destination IPs: {}'.format(len(output)/n_destinations))


# In[40]:


avg_duration = (output.groupby(['Source','Destination'])['Time'].max() - output.groupby(['Source','Destination'])['Time'].min()).mean()
print('Average flow duration: {}'.format(avg_duration))


# In[41]:


print('Average no of Packets received per flow: {}'.format(output[output['Destination']==IP].groupby(['Source','Destination'])['Length'].count().mean()))


# In[42]:


print('Average no of Bytes received per flow: {}'.format(output[output['Destination']==IP].groupby(['Source','Destination'])['Length'].sum().mean()))


# In[43]:


ans = sum(output[output['Destination']==IP]['Length'])/sum(output[output['Source']==IP]['Length'])
print('Average ratio of incoming to outgoing bytes: {}'.format(ans))


# In[44]:


x = (output[output['Destination']==IP]['Time'])
ans = (x.iloc[len(x)-1] - x.iloc[0])/len(x)
print('Average Time interval between packets received: {}'.format(ans))

