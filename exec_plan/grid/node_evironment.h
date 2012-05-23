<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<head>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<title>node_evironment.h - Column oriented data warehousing - dev.MIM</title>
<meta name="description" content="Redmine" />
<meta name="keywords" content="issue,bug,tracker" />
<meta name="csrf-param" content="authenticity_token"/>
<meta name="csrf-token" content="fZWtE6aoce84bZJFerhrTO/V9xIRDBjVDORRO+f0zLc="/>
<link rel='shortcut icon' href='/favicon.ico?1323697841' />
<link href="/stylesheets/application.css?1323697841" media="all" rel="stylesheet" type="text/css" />

<script src="/javascripts/prototype.js?1323697841" type="text/javascript"></script>
<script src="/javascripts/effects.js?1323697841" type="text/javascript"></script>
<script src="/javascripts/dragdrop.js?1323697841" type="text/javascript"></script>
<script src="/javascripts/controls.js?1323697841" type="text/javascript"></script>
<script src="/javascripts/application.js?1323697841" type="text/javascript"></script>
<script type="text/javascript">
//<![CDATA[
Event.observe(window, 'load', function(){ new WarnLeavingUnsaved('The current page contains unsaved text that will be lost if you leave this page.'); });
//]]>
</script>

<!--[if IE 6]>
    <style type="text/css">
      * html body{ width: expression( document.documentElement.clientWidth < 900 ? '900px' : '100%' ); }
      body {behavior: url(/stylesheets/csshover.htc?1323697841);}
    </style>
<![endif]-->

<!-- page specific tags -->
    <link href="/stylesheets/scm.css?1323697841" media="screen" rel="stylesheet" type="text/css" /></head>
<body class="controller-attachments action-show">
<div id="wrapper">
<div id="wrapper2">
<div id="top-menu">
    <div id="account">
        <ul><li><a href="/login" class="login">Sign in</a></li></ul>    </div>
    
    <ul><li><a href="/" class="home">Home</a></li>
<li><a href="/projects" class="projects">Projects</a></li>
<li><a href="http://www.redmine.org/guide" class="help">Help</a></li></ul></div>

<div id="header">
    
    <div id="quick-search">
        <form action="/search/index/codwh" method="get">
        
        <label for='q'>
          <a href="/search/index/codwh" accesskey="4">Search</a>:
        </label>
        <input accesskey="f" class="small" id="q" name="q" size="20" type="text" />
        </form>
        
    </div>
    

    <h1>Column oriented data warehousing</h1>

    
    <div id="main-menu">
        <ul><li><a href="/projects/codwh" class="overview">Overview</a></li>
<li><a href="/projects/codwh/activity" class="activity">Activity</a></li>
<li><a href="/projects/codwh/wiki" class="wiki">Wiki</a></li>
<li><a href="/projects/codwh/files" class="files">Files</a></li></ul>
    </div>
    
</div>

<div class="nosidebar" id="main">
    <div id="sidebar">
        
        
    </div>

    <div id="content">
        
        <h2>node_evironment.h</h2>

<div class="attachments">
<p>
   <span class="author"><a href="/users/193">Piotr Tabor</a>, 04/19/2012 07:17 pm</span></p>
<p><a href="/attachments/download/376/node_evironment.h">Download</a>   <span class="size">(4 kB)</span></p>

</div>
&nbsp;
<div class="autoscroll">
<table class="filecontent syntaxhl">
<tbody>


  <tr>
    <th class="line-num" id="L1">
      <a href="#L1">1</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// Copyright 2012 Google Inc. All Rights Reserved.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L2">
      <a href="#L2">2</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// Author: onufry@google.com (Onufry Wojtaszczyk)</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L3">
      <a href="#L3">3</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">//         ptab@google.com (Piotr Tabor)</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L4">
      <a href="#L4">4</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L5">
      <a href="#L5">5</a>
    </th>
    <td class="line-code">
      <pre><span class="preprocessor">#include</span> <span class="include">&lt;stdint.h&gt;</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L6">
      <a href="#L6">6</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L7">
      <a href="#L7">7</a>
    </th>
    <td class="line-code">
      <pre><span class="keyword">typedef</span> int32_t int32;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L8">
      <a href="#L8">8</a>
    </th>
    <td class="line-code">
      <pre><span class="keyword">typedef</span> uint32_t uint32;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L9">
      <a href="#L9">9</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L10">
      <a href="#L10">10</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// DataSourceInterface and DataSinkInterface are the equivalent of the Server</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L11">
      <a href="#L11">11</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// interface from the previous assignment.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L12">
      <a href="#L12">12</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L13">
      <a href="#L13">13</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// DataSourceInterface represents an abstraction of a single source file with</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L14">
      <a href="#L14">14</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// data that need to be processed by the database.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L15">
      <a href="#L15">15</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// The DataSourceInterface implementation is not guaranteed to be thread-safe.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L16">
      <a href="#L16">16</a>
    </th>
    <td class="line-code">
      <pre>class DataSourceInterface {
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L17">
      <a href="#L17">17</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Fills |destination| with at most |number| entries, which are the subsequent</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L18">
      <a href="#L18">18</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// entries from column |column_index|. Returns the number of entries filled</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L19">
      <a href="#L19">19</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// out. A return value of zero should be interpreted as End Of Stream.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L20">
      <a href="#L20">20</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Unpredictable behaviour if trying to read other data type than what was</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L21">
      <a href="#L21">21</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// declared by the source in the plan.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L22">
      <a href="#L22">22</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="predefined-type">int</span> GetDoubles(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number, <span class="predefined-type">double</span>* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L23">
      <a href="#L23">23</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="predefined-type">int</span> GetInts(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number, int32* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L24">
      <a href="#L24">24</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Bools in the standard C++ format.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L25">
      <a href="#L25">25</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="predefined-type">int</span> GetByteBools(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number, <span class="predefined-type">bool</span>* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L26">
      <a href="#L26">26</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Bools encoded as bitmasks.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L27">
      <a href="#L27">27</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="predefined-type">int</span> GetBitBools(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number, <span class="predefined-type">char</span>* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L28">
      <a href="#L28">28</a>
    </th>
    <td class="line-code">
      <pre>};
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L29">
      <a href="#L29">29</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L30">
      <a href="#L30">30</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// DataSinkInterface represents an abstraction of a single result file produced</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L31">
      <a href="#L31">31</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// by the database. The produced data will be checked for correctness.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L32">
      <a href="#L32">32</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// Please note that the query is considered to be finished (the time stops) when</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L33">
      <a href="#L33">33</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// all the nodes exited with status = 0. </span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L34">
      <a href="#L34">34</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// The sink implementation is not guaranted to be thread-safe.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L35">
      <a href="#L35">35</a>
    </th>
    <td class="line-code">
      <pre>class DataSinkInterface {
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L36">
      <a href="#L36">36</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Consumes the results of the calculation. The first |number| entries in</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L37">
      <a href="#L37">37</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// |destination| are assumed to be the subsequent entries in column</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L38">
      <a href="#L38">38</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// |column_index|.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L39">
      <a href="#L39">39</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="directive">void</span> ConsumeDoubles(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number,
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L40">
      <a href="#L40">40</a>
    </th>
    <td class="line-code">
      <pre>                              <span class="directive">const</span> <span class="predefined-type">double</span>* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L41">
      <a href="#L41">41</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="directive">void</span> ConsumeInts(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number,
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L42">
      <a href="#L42">42</a>
    </th>
    <td class="line-code">
      <pre>                           <span class="directive">const</span> int32* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L43">
      <a href="#L43">43</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="directive">void</span> ConsumeByteBools(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number,
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L44">
      <a href="#L44">44</a>
    </th>
    <td class="line-code">
      <pre>                                <span class="directive">const</span> <span class="predefined-type">bool</span>* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L45">
      <a href="#L45">45</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="directive">void</span> ConsumeBitBools(<span class="predefined-type">int</span> column_index, <span class="predefined-type">int</span> number,
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L46">
      <a href="#L46">46</a>
    </th>
    <td class="line-code">
      <pre>                               <span class="directive">const</span> <span class="predefined-type">char</span>* destination) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L47">
      <a href="#L47">47</a>
    </th>
    <td class="line-code">
      <pre>};
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L48">
      <a href="#L48">48</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L49">
      <a href="#L49">49</a>
    </th>
    <td class="line-code">
      <pre><span class="comment">// NodeEnvironmentInterface implementation is thread-safe.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L50">
      <a href="#L50">50</a>
    </th>
    <td class="line-code">
      <pre>class NodeEnvironmentInterface {
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L51">
      <a href="#L51">51</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Returns node number of the process.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L52">
      <a href="#L52">52</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="predefined-type">int</span> get_my_node_number() <span class="directive">const</span> = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L53">
      <a href="#L53">53</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L54">
      <a href="#L54">54</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// -------------- Inter-worker communication ---------------------------------</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L55">
      <a href="#L55">55</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L56">
      <a href="#L56">56</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Sends a packet to given |target_node|. The packet</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L57">
      <a href="#L57">57</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// will be returned on the |target_node| from one</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L58">
      <a href="#L58">58</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// of the subsequent ReadPacket* calls. Note that</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L59">
      <a href="#L59">59</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// this call can block, if network buffers are full</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L60">
      <a href="#L60">60</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// (e.g. consumer is not reading packets).</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L61">
      <a href="#L61">61</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// There will be no interleaving between packets and two packets </span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L62">
      <a href="#L62">62</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// will not arrive as a single packet.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L63">
      <a href="#L63">63</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="directive">void</span> SendPacket(uint32 target_node,
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L64">
      <a href="#L64">64</a>
    </th>
    <td class="line-code">
      <pre>                          <span class="directive">const</span> <span class="predefined-type">char</span>* data,
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L65">
      <a href="#L65">65</a>
    </th>
    <td class="line-code">
      <pre>                          <span class="predefined-type">int</span>  data_len) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L66">
      <a href="#L66">66</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L67">
      <a href="#L67">67</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Reads a single packet sent to this node.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L68">
      <a href="#L68">68</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Blocks if there is not packet ready until the packet arrive.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L69">
      <a href="#L69">69</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Caller takes ownership of the returned packet and should</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L70">
      <a href="#L70">70</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// destroy it using delete[].</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L71">
      <a href="#L71">71</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="predefined-type">char</span>* ReadPacketBlocking(<span class="predefined-type">int</span>* data_len) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L72">
      <a href="#L72">72</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L73">
      <a href="#L73">73</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Returns NULL if there is no packet waiting</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L74">
      <a href="#L74">74</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Updates data_len to contain the size of the read packet.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L75">
      <a href="#L75">75</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Caller takes ownership of the returned packet and should</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L76">
      <a href="#L76">76</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// destroy it using delete[].</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L77">
      <a href="#L77">77</a>
    </th>
    <td class="line-code">
      <pre>  virtual <span class="predefined-type">char</span>* ReadPacketNotBlocking(<span class="predefined-type">int</span>* data_len) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L78">
      <a href="#L78">78</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L79">
      <a href="#L79">79</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// -------- Reading input files and writing results --------------------------</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L80">
      <a href="#L80">80</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L81">
      <a href="#L81">81</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Caller takes ownership of the source.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L82">
      <a href="#L82">82</a>
    </th>
    <td class="line-code">
      <pre>  virtual DataSourceInterface* OpenDataSourceFile(<span class="predefined-type">int</span> source_file_id) = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L83">
      <a href="#L83">83</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L84">
      <a href="#L84">84</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// Caller takes ownership of the sink.</span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L85">
      <a href="#L85">85</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// You should open a single sink only once query-wide </span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L86">
      <a href="#L86">86</a>
    </th>
    <td class="line-code">
      <pre>  <span class="comment">// (unpredictable behaviour otherwise). </span>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L87">
      <a href="#L87">87</a>
    </th>
    <td class="line-code">
      <pre>  virtual DataSinkInterface* OpenDataSink() = <span class="integer">0</span>;
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L88">
      <a href="#L88">88</a>
    </th>
    <td class="line-code">
      <pre>};
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L89">
      <a href="#L89">89</a>
    </th>
    <td class="line-code">
      <pre>
</pre>
    </td>
  </tr>
  

  <tr>
    <th class="line-num" id="L90">
      <a href="#L90">90</a>
    </th>
    <td class="line-code">
      <pre>NodeEnvironmentInterface* CreateNodeEnvironment(<span class="predefined-type">int</span> argc, <span class="predefined-type">char</span>** argv);
</pre>
    </td>
  </tr>
  

</tbody>
</table>
</div>





        
        <div style="clear:both;"></div>
    </div>
</div>

<div id="ajax-indicator" style="display:none;"><span>Loading...</span></div>

<div id="footer">
  <div class="bgl"><div class="bgr">
    Powered by <a href="http://www.redmine.org/">Redmine</a> &copy; 2006-2011 Jean-Philippe Lang
  </div></div>
</div>
</div>
</div>

</body>
</html>
